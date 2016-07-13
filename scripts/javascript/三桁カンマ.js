WScript.Echo("1".replace(/(\d)(?=(\d{3})+$)/g, "$1,"));
WScript.Echo("123".replace(/(\d)(?=(\d{3})+$)/g, "$1,"));
WScript.Echo("1234".replace(/(\d)(?=(\d{3})+$)/g, "$1,"));
WScript.Echo("1234567890".replace(/(\d)(?=(\d{3})+$)/g, "$1,"));


