from setuptools import find_packages, setup

package_name = "auv_mav_utils"
setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="jalan",
    maintainer_email="subham.jalan@plaksha.edu.in",
    description="TODO: Package description",
    license="TODO: License declaration",
    entry_points={
        "console_scripts": [
            "depth_publisher = auv_mav_utils.depth_publisher:main",
            "calibrate = auv_mav_utils.calibrate:main",
            "heading_test = auv_mav_utils.heading_test:main",
        ],
    },
)
