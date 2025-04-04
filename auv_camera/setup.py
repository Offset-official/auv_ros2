from setuptools import find_packages, setup
import os
from glob import glob

package_name = "auv_camera"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=[],
    zip_safe=True,
    maintainer="Subham Jalan",
    maintainer_email="subham.jalan@plaksha.edu.in",
    description="This package acts as the eyes of the AUV.",
    license="TODO: License declaration",
    extras_require={
        "test": ["pytest"],  # or other testing dependencies
    },
    entry_points={
        "console_scripts": [
            # "yolo_inference_server_gate = auv_ml.yolo_inference_server_gate:main",
            # "yolo_inference_test_gate = auv_ml.yolo_inference_test_gate:main",
            "publish_camera = auv_camera.publish_camera:main",
            "camera_recorder = auv_camera.camera_recorder:main",
        ],
    },
)
