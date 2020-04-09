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