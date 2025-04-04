from setuptools import find_packages, setup

package_name = 'auv_diagnostics'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', ['launch/diagnostics_launch.py']),
    ],
    install_requires=['setuptools', 'pyserial', 'opencv-python', 'numpy'],
    zip_safe=True,
    maintainer='and-human',
    maintainer_email='Anshuman Sharma',
    description='TODO: Package description',
    license='TODO: License declaration',
    entry_points={
        'console_scripts': [
            'diagnostics = auv_diagnostics.diagnostics:main',
            'serial_communication = auv_diagnostics.serial_communication:main',
        ],
    },
)
