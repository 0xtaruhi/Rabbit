#include <QApplication>
#include <QDebug>

#include "FPGA/VLFDDeviceDetector.h"
#include "libusb.h"

using namespace rabbit_App::fpga;

VLFDDeviceDetector *VLFDDeviceDetector::instance_ = nullptr;

VLFDDeviceDetector::VLFDDeviceDetector(QObject *parent) : QObject(parent) {
  instance_ = this;
  // thread_ = new QThread(this);
  // thread_->setObjectName("VLFDDeviceDetectorThread");
  // timer_ = new QTimer(this);
  // timer_->setInterval(kTimerInterval);
  // timer_->moveToThread(thread_);
  // connect(timer_, &QTimer::timeout, this,
  // &VLFDDeviceDetector::onTimerTimeOut,
  //         Qt::DirectConnection);
  // connect(timer_, &QTimer::timeout, this, &VLFDDeviceDetector::onTimerTimeOut);
  // connect(thread_, &QThread::started, timer_,
  // QOverload<>::of(&QTimer::start)); connect(thread_, &QThread::finished,
  // timer_, &QTimer::stop); connect(thread_, &QThread::finished, this,
  // &QObject::deleteLater());
  time_thread_ = new TimeThreadWorker(new DetectWorker(this), this,
                                      Qt::QueuedConnection);
  time_thread_->setInterval(kTimerInterval);
}

VLFDDeviceDetector::~VLFDDeviceDetector() {
  // stopDetect();
  // if (timer_ != nullptr && timer_->isActive()) {
  //   timer_->stop();
  // }
  // timer_->deleteLater();
  // if (thread_->isRunning()) {
  //   thread_->quit();
  // }
  // thread_->wait();
  // // thread_->deleteLater();
  // delete thread_;
}

void VLFDDeviceDetector::startDetect() {
  if (is_detecting_) {
    return;
  }
  device_connected_ = deviceExists();
  detectArrived(device_connected_);
  detectLeft(!device_connected_);
  is_detecting_ = true;
  // this->moveToThread(thread_);
  // thread_->start();
  // timer_->start();
  time_thread_->start();
}

void VLFDDeviceDetector::stopDetect() {
  if (!is_detecting_) {
    return;
  }
  is_detecting_ = false;
  // this->moveToThread(QApplication::instance()->thread());
  // timer_->stop();
  time_thread_->stop();
  // thread_->quit();
}

void VLFDDeviceDetector::detectArrived(bool arrived) {
  if (arrived) {
    device_connected_ = true;
    // qDebug() << "Device connected";
    emit deviceDetected();
  }
}
void VLFDDeviceDetector::detectLeft(bool left) {
  if (left) {
    device_connected_ = false;
    // qDebug() << "Device disconnected";
    emit deviceRemoved();
  }
}

bool VLFDDeviceDetector::deviceExists() {
  libusb_context *ctx = NULL;
  if (LIBUSB_SUCCESS != libusb_init(&ctx)) {
    return false;
  }
  libusb_device **devs;
  ssize_t cnt = libusb_get_device_list(ctx, &devs);
  bool found = false;
  for (ssize_t i = 0; i < cnt; i++) {
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(devs[i], &desc);
    if (desc.idVendor == kVendorID && desc.idProduct == kProductID) {
      found = true;
      break;
    }
  }
  libusb_free_device_list(devs, 1);
  libusb_exit(ctx);
  return found;
}

LibusbVLFDDeviceDetector::LibusbVLFDDeviceDetector(QObject *parent)
    : VLFDDeviceDetector(parent) {
  context_ = NULL;
  if (LIBUSB_SUCCESS != libusb_init(&context_)) {
    throw(tr("Failed to initialize libusb"));
  }
  if (0 == libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
    throw(tr("Hotplug capabilites are not supported on this platform"));
  }
  if (LIBUSB_SUCCESS != libusb_hotplug_register_callback(
                            context_, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
                            hotplug_flag, kVendorID, kProductID,
                            LIBUSB_HOTPLUG_MATCH_ANY, arrivedCallback, this,
                            &callback_handle[0])) {
    throw(tr("Failed to register hotplug callback for device arrival"));
  }
  if (LIBUSB_SUCCESS != libusb_hotplug_register_callback(
                            context_, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
                            hotplug_flag, kVendorID, kProductID,
                            LIBUSB_HOTPLUG_MATCH_ANY, leftCallback, this,
                            &callback_handle[1])) {
    throw(tr("Failed to register hotplug callback for device removal"));
  }
}

LibusbVLFDDeviceDetector::~LibusbVLFDDeviceDetector() {
  libusb_hotplug_deregister_callback(context_, callback_handle[0]);
  libusb_hotplug_deregister_callback(context_, callback_handle[1]);
  libusb_exit(context_);
  stopDetect();
}

void LibusbVLFDDeviceDetector::onTimerTimeOut() {
  timeval tv = {0, 0};
  libusb_handle_events_timeout(NULL, &tv);
}

int LibusbVLFDDeviceDetector::arrivedCallback(libusb_context *ctx,
                                              libusb_device *dev,
                                              libusb_hotplug_event event,
                                              void *user_data) {
  auto detector = static_cast<VLFDDeviceDetector *>(user_data);
  libusb_device_descriptor *desc = new libusb_device_descriptor();
  libusb_get_device_descriptor(dev, desc);
  detector->detectArrived(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED &&
                          desc->idVendor == kVendorID &&
                          desc->idProduct == kProductID);
  delete desc;
  return 0;
}

int LibusbVLFDDeviceDetector::leftCallback(libusb_context *ctx,
                                           libusb_device *dev,
                                           libusb_hotplug_event event,
                                           void *user_data) {
  auto detector = static_cast<VLFDDeviceDetector *>(user_data);
  detector->detectLeft(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
  return 0;
}

#ifdef _WIN32
#include <dbt.h>
#include <initguid.h>
#include <usbiodef.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

WinVLFDDeviceDetector::WinVLFDDeviceDetector(QObject *parent)
    : VLFDDeviceDetector(parent) {
  hWnd = NULL;
  ZeroMemory(&wx, sizeof(wx));

  wx.cbSize = sizeof(WNDCLASSEX);
  wx.lpfnWndProc = reinterpret_cast<WNDPROC>(message_handler);
  wx.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
  wx.style = CS_HREDRAW | CS_VREDRAW;
  wx.hInstance = GetModuleHandle(0);
  wx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wx.lpszClassName = "DUMMY_CLASS";
  if (RegisterClassEx(&wx)) {
    hWnd = CreateWindow("DUMMY_CLASS", "DevNotifWnd", WS_ICONIC, 0, 0,
                        CW_USEDEFAULT, 0, ((HWND)-3), NULL, GetModuleHandle(0),
                        (void *)&guid);
  }
}

WinVLFDDeviceDetector::~WinVLFDDeviceDetector() {
  stopDetect();
  DestroyWindow(hWnd);
  UnregisterClass("DUMMY_CLASS", GetModuleHandle(0));
}

void WinVLFDDeviceDetector::onTimerTimeOut() {
  // qDebug() << "WinVLFDDeviceDetector thread: " << QThread::currentThreadId();
  MSG msg;
  GetMessage(&msg, NULL, 0, 0);
  TranslateMessage(&msg);
  DispatchMessage(&msg);
}

LRESULT CALLBACK WinVLFDDeviceDetector::message_handler(HWND__ *hwnd, UINT uint,
                                                        WPARAM wparam,
                                                        LPARAM lparam) {
  switch (uint) {
  case WM_CREATE: // the actual creation of the window
  {
    LPCREATESTRUCT params = (LPCREATESTRUCT)lparam;
    GUID InterfaceClassGuid = *((GUID *)params->lpCreateParams);
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    memcpy(&(NotificationFilter.dbcc_classguid),
           &(GUID_DEVINTERFACE_USB_DEVICE), sizeof(struct _GUID));
    HDEVNOTIFY dev_notify = RegisterDeviceNotification(
        hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
    if (dev_notify == NULL) {
      throw std::runtime_error("Could not register for devicenotifications!");
    }
    break;
  }
  case WM_DEVICECHANGE: {
    PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lparam;
    PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
    if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
      VLFDDeviceDetector::instance()->detectArrived(wparam ==
                                                    DBT_DEVICEARRIVAL);
      VLFDDeviceDetector::instance()->detectLeft(wparam ==
                                                 DBT_DEVICEREMOVECOMPLETE);
    }
    break;
  }
  default:
    return true;
  }
  return 0L;
}

#endif // ifdef _WIN32

DetectWorker::DetectWorker(VLFDDeviceDetector *detector) : detector_(detector) {}

DetectWorker::~DetectWorker() {}

void DetectWorker::doWork() {
  // qDebug() << "DetectWorker::doWork() thread: " <<
  // QThread::currentThreadId();
  detector_->onTimerTimeOut();
}