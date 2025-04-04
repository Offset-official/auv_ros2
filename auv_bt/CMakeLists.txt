cmake_minimum_required(VERSION 3.8)
project(auv_bt)

set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(auv_interfaces REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(mavros_msgs REQUIRED)
find_package(std_srvs REQUIRED)

find_package(ZMQ)
if(ZMQ_FOUND)
    message(STATUS "ZeroMQ found.")
    add_definitions(-DZMQ_FOUND)
else()
  message(WARNING "ZeroMQ NOT found. Not including PublisherZMQ.")
endif()

set(CMAKE_CXX_STANDARD 17)


set(dependencies
    rclcpp
    rclcpp_action
    behaviortree_cpp_v3
    sensor_msgs
    geometry_msgs
    auv_interfaces
    mavros_msgs
    std_srvs
    ament_index_cpp
)

include_directories(include ${ZMQ_INCLUDE_DIRS})

add_library(auv_elapsed_bt_node SHARED src/auv_bt/Elapsed.cpp)
add_library(auv_forward_bt_node SHARED src/auv_bt/Forward.cpp)
add_library(auv_turn_bt_node SHARED src/auv_bt/Turn.cpp)
add_library(auv_descend_bt_node SHARED src/auv_bt/Descend.cpp)
add_library(auv_set_led_color_bt_node SHARED src/auv_bt/SetLEDColor.cpp)
add_library(auv_set_mode_bt_node SHARED src/auv_bt/SetMode.cpp)
add_library(auv_is_armed_bt_node SHARED src/auv_bt/isArmed.cpp)
add_library(auv_gate_bt_node SHARED src/auv_bt/Gate.cpp)
add_library(auv_follow_gate_bt_node SHARED src/auv_bt/FollowGate.cpp)
add_library(auv_is_depth_calibrated_bt_node SHARED src/auv_bt/isDepthCalibrated.cpp)
add_library(auv_calibrate_depth_bt_node SHARED src/auv_bt/CalibrateDepth.cpp)

list(APPEND plugin_libs
    auv_elapsed_bt_node
    auv_forward_bt_node
    auv_turn_bt_node
    auv_descend_bt_node
    auv_set_led_color_bt_node
    auv_set_mode_bt_node
    auv_is_armed_bt_node
    auv_gate_bt_node
    auv_follow_gate_bt_node
    auv_is_depth_calibrated_bt_node
    auv_calibrate_depth_bt_node
)

foreach(bt_plugin ${plugin_libs})
  ament_target_dependencies(${bt_plugin} ${dependencies})
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

add_executable(bt_qualify src/bt_qualify_main.cpp)
ament_target_dependencies(bt_qualify ${dependencies})
target_link_libraries(bt_qualify ${ZMQ_LIBRARIES})

install(TARGETS
  ${plugin_libs}
  bt_qualify
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

install(DIRECTORY behavior_tree_xml
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_export_dependencies(${dependencies})

ament_package()
