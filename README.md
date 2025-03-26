Smart Home Service used to control appliances in my house.

Integrated Appliances:
Motorised Curtains
Light Switches (XiaoMi Hub)

Dependencies:
-python
-miio python library
-pybind11
-uWebSockets
-nlohmann json

The server layer is written in C++, serving using uWebSockets. However, most intra rest calls to appliances use the python library miio. Since miio is in python, pybind11 is used to interface between the C++ server and python rest calls.

