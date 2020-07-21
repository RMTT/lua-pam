## lua-pam
A module for lua  to use PAM.

## Complile
```shell
cmake . -B build
cd build
make
```

## Usage
```lua
-- add the lib to cpath by yourself

local pam = require("liblua_pam")
local auth = pam.auth_current_user(password)

if auth
then
    print("Success!")
end
```

## Troubleshooting
### cannot find `lua.hpp`
Firstly,make sure the `lua.hpp` exists on your machine,and then create a soft link to the `/usr/include` directory,also,you can change the `CMakeLists.txt` to include your own directory which contains `lua.hpp`
### cannot find liblua.so
Make sure the file exists on your machine,it's need install some package on some distros,such as Debian/Ubuntu,you should install liblua-dev
### cannot find `pam_appl.hpp` on Ubuntu/Debian
Install `libpam0g-dev`
