用[WebKitForWindows/WebKitRequirements a8c7798b3751d63d6b160df431031cd5e5c940d9](https://github.com/WebKitForWindows/WebKitRequirements)的curl时，
如果同时开启 CURLOPT_DEBUGFUNCTION 和 CURLOPT_STDERR, 则 CURLOPT_STDERR 会被忽视。
如果用原版则不会（测试版本curl7.87.0）。
