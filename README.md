## lua-pam
A module for lua to use PAM.

## Compile
```shell
cmake . -B build
cd build
make
```

## Installation through package manager

### Arch Linux

The package [`lua-pam-git`](https://aur.archlinux.org/packages/lua-pam-git/) can be installed from the Arch Linux User Repository using the following command. The command assumes your package manager is `yay`.

```
yay -S lua-pam-git
```

The file `liblua_pam.so` is installed into `/usr/lib/lua-pam/`.

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
#### cannot find `-llua`
Check for lua libraries in `/usr/lib`. If a library exists like `liblua5.4.so` or `libluajit-5.1.so`, change `lua` in `CMakeLists.txt` to use the library you have installed (in the example libraries before, this would be `lua5.4` and `luajit-5.1` respectively).
#### cannot find `lua.hpp`
Make sure that `lua.hpp` exists on your machine, and then create a soft link to the `/usr/include` directory if it isn't there already. You can change `CMakeLists.txt` to include your own directory which contains `lua.hpp`.
#### cannot find `liblua.so`
Make sure the file exists on your machine. A package will need to be installed on some distros. For distros such as Debian/Ubuntu, you should install `liblua-dev`.
#### cannot find `pam_appl.hpp` on Ubuntu/Debian
Install `libpam0g-dev`.
