#ifndef VLFDDEVICEDETECTOR_H
#define VLFDDEVICEDETECTOR_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "ThreadTimer.h"
#include "libusb.h"

namespace rabbit_App::fpga {

const uint16_t kVendorID = 0x2200;
const uint16_t kProductID = 0x2008;
const QString kDeviceName = "VLX2";

/// @brief VLFDDeviceDetector class
/// This class is used to detect the connection of the VLFD device.
/// To avoid the gui blocking, we use a worker to process the event.
/// Because the libusb hotplug is not supported on Windows, so in windows
/// platform, we use windows message to detect the connection.
/// In other platform, we use libusb hotplug to detect the connection.
class VLFDDeviceDetector : public QObject {
  Q_OBJECT

  friend class DetectWorker;

public:
  constexpr static int kTimerInterval = 1000;

public:
  VLFDDeviceDetector(QObject *parent = nullptr);
  virtual ~VLFDDeviceDetector();

  /// @brief The function called when the timer timeout.
  virtual void onTimerTimeOut() {}

  void startDetect();
  void stopDetect();

  bool isConnected() const noexcept { return device_connected_; }
  void setConnected(bool connected) noexcept { device_connected_ = connected; }
  void detectArrived(bool arrived);
  void detectLeft(bool left);

  static bool deviceExists();
  static VLFDDeviceDetector *instance() { return instance_; }

signals:
  void deviceDetected();
  void deviceRemoved();

protected:
  // QThread *thread_;
  // QTimer *timer_;

  /// @brief The worker manager to process the detect event.
  TimeThreadWorker *time_thread_;
  bool is_detecting_ = false;
  bool device_connected_ = false;

  /// @brief static instance of the VLFDDeviceDetector.
  /// We use a static instance to connect to a virtual window on windows
  /// platform.
  static VLFDDeviceDetector *instance_;

}; // class VLFDDeviceDetector

class LibusbVLFDDeviceDetector : public VLFDDeviceDetector {
  Q_OBJECT

  constexpr static int hotplug_flag = LIBUSB_HOTPLUG_ENUMERATE;

public:
  LibusbVLFDDeviceDetector(QObject *parent = nullptr);
  ~LibusbVLFDDeviceDetector();

  void onTimerTimeOut() override;

private:
  /// @brief The callback function called when the device arrived.
  static int LIBUSB_CALL arrivedCallback(libusb_context *ctx,
                                         libusb_device *dev,
                                         libusb_hotplug_event event,
                                         void *user_data);
  /// @brief The callback function called when the device left.
  static int LIBUSB_CALL leftCallback(libusb_context *ctx, libusb_device *dev,
                                      libusb_hotplug_event event,
                                      void *user_data);

private:
  libusb_context *context_;
  libusb_hotplug_callback_handle callback_handle[2];

}; // class LibusbVLFDDeviceDetector

#ifdef _WIN32

#include <windows.h>

class WinVLFDDeviceDetector : public VLFDDeviceDetector {

  /// @brief The GUID of the device interface class for VLFD device.
  /// {88bae032-5a81-49f0-bc3d-a4ff138216d6}
  constexpr static GUID guid = {
      0x88bae032,
      0x5a81,
      0x49f0,
      {0xbc, 0x3d, 0xa4, 0xff, 0x13, 0x82, 0x16, 0xd6}};

public:
  WinVLFDDeviceDetector(QObject *parent = nullptr);
  ~WinVLFDDeviceDetector();

  void onTimerTimeOut() override;

private:
  /// @brief The message handler of the window.
  static LRESULT CALLBACK message_handler(HWND__ *hwnd, UINT uint,
                                          WPARAM wparam, LPARAM lparam);

private:
  HWND hWnd;
  WNDCLASSEX wx;

}; // class WinVLFDDeviceDetector

#endif // ifdef _WIN32

/// @brief DetectWorker class used to process the detect event.
class DetectWorker : public Worker {
  Q_OBJECT

public:
  DetectWorker(VLFDDeviceDetector *detector);
  ~DetectWorker();

protected:
  void doWork() override;

private:
  VLFDDeviceDetector *detector_;

}; // class DetectWorker

} // namespace rabbit_App::fpga

#endif // VLFDDEVICEDETECTOR_H
