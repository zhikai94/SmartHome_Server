Smart Home Service used to control appliances in my house.

Integrated Appliances:<br>
Motorised Curtains<br>
Light Switches (XiaoMi Hub)<br>

Dependencies:<br>
-python<br>
-miio python library<br>
-pybind11<br>
-uWebSockets<br>
-nlohmann json<br>

The server layer is written in C++, serving using uWebSockets. However, most intra rest calls to appliances use the python library miio. Since miio is in python, pybind11 is used to interface between the C++ server and python rest calls.

