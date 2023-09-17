#include "Ports/PinInfo.h"
#include "Ports/Port.h"
#include <QHash>

using namespace rabbit_App::ports;

const QString rabbit_App::ports::portTypeToString(const PortType type) {
  switch (type) {
  case PortType::Input:
    return "Input";
  case PortType::Output:
    return "Output";
  case PortType::Clock:
    return "Clock";
  default:
    return "Undefined";
  }
}

const PortType rabbit_App::ports::stringToPortType(const QString &type) {
  if (type == "Input") {
    return PortType::Input;
  } else if (type == "Output") {
    return PortType::Output;
  } else if (type == "Clock") {
    return PortType::Clock;
  } else {
    return PortType::Undefined;
  }
}

static const QHash<QString, int> input_decl_index_map{
    {"P151", 0},  {"P148", 1},  {"P150", 2},  {"P152", 3},  {"P160", 4},
    {"P161", 5},  {"P162", 6},  {"P163", 7},  {"P164", 8},  {"P165", 9},
    {"P166", 10}, {"P169", 11}, {"P173", 12}, {"P174", 13}, {"P175", 14},
    {"P191", 15}, {"P120", 16}, {"P116", 17}, {"P115", 18}, {"P114", 19},
    {"P113", 20}, {"P112", 21}, {"P111", 22}, {"P108", 23}, {"P102", 24},
    {"P101", 25}, {"P100", 26}, {"P97", 27},  {"P96", 28},  {"P95", 29},
    {"P89", 30},  {"P88", 31},  {"P87", 32},  {"P86", 33},  {"P81", 34},
    {"P75", 35},  {"P74", 36},  {"P70", 37},  {"P69", 38},  {"P68", 39},
    {"P64", 40},  {"P62", 41},  {"P61", 42},  {"P58", 43},  {"P57", 44},
    {"P49", 45},  {"P47", 46},  {"P48", 47},  {"P192", 48}, {"P193", 49},
    {"P199", 50}, {"P200", 51}, {"P201", 52}, {"P202", 53}};

static const QHash<QString, int> output_decl_index_map{
    {"P7", 1},    {"P6", 2},    {"P5", 3},    {"P4", 4},    {"P9", 5},
    {"P8", 6},    {"P16", 7},   {"P15", 8},   {"P11", 9},   {"P10", 10},
    {"P20", 11},  {"P18", 12},  {"P17", 13},  {"P22", 14},  {"P21", 15},
    {"P23", 16},  {"P44", 17},  {"P45", 18},  {"P46", 19},  {"P43", 20},
    {"P40", 21},  {"P41", 22},  {"P42", 23},  {"P33", 24},  {"P34", 25},
    {"P35", 26},  {"P36", 27},  {"P30", 28},  {"P31", 29},  {"P24", 30},
    {"P27", 31},  {"P29", 32},  {"P110", 33}, {"P109", 34}, {"P99", 35},
    {"P98", 36},  {"P94", 37},  {"P93", 38},  {"P84", 39},  {"P83", 40},
    {"P82", 41},  {"P73", 42},  {"P71", 43},  {"P63", 44},  {"P60", 45},
    {"P59", 46},  {"P56", 47},  {"P55", 48},  {"P167", 49}, {"P168", 50},
    {"P176", 51}, {"P187", 52}, {"P189", 53}, {"P194", 54}};

static const QString clock_decl = "";

auto rabbit_App::ports::inputDeclIndexMap(const QString &pin_name) -> int {
  if (auto iter = input_decl_index_map.find(pin_name);
      iter != input_decl_index_map.end()) {
    return iter.value();
  }
  return -1;
}

auto rabbit_App::ports::outputDeclIndexMap(const QString &pin_name) -> int {
  if (auto iter = output_decl_index_map.find(pin_name);
      iter != output_decl_index_map.end()) {
    return iter.value();
  }
  return -1;
}

auto rabbit_App::ports::getClockPinDecl() -> QString { return clock_decl; }

auto rabbit_App::ports::declIndexMap(const QString &pin_name)
    -> QPair<PortType, int> {
  if (pin_name == clock_decl) {
    return {PortType::Clock, 0};
  }
  if (auto iter = input_decl_index_map.find(pin_name);
      iter != input_decl_index_map.end()) {
    return {PortType::Input, iter.value()};
  }
  if (auto iter = output_decl_index_map.find(pin_name);
      iter != output_decl_index_map.end()) {
    return {PortType::Output, iter.value()};
  }
  return {PortType::Undefined, -1};
}
