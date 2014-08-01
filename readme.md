# wxWebViewChromium

wxWebViewChromium is a Chromium backend for wxWebView using the Chromium
Embedded Framework(CEF3).

## Requirements

[Chromium Embedded Framework][1]: 3.1750.1738

* Windows: 3.1750.1738 or 3.1650.1562
* Linux: 3.1750.1738 or 3.1650.1562
* Mac OS X: 3.1750.1738

On Windows platform, you will need to change the vistual stdio building
properties of `libcef_dll_wrapper` to consistent with wxWidgets, see following steps:

  * C/C++ - General - Treat Warngings as Error - No
  * C/C++ - Code Generation - Runtime Library - Multithreaded [Debug] DLL
  * C/C++ - Code Generation - Enable C++ Exceptions - Yes
  * C/C++ - Language - Enable Run-Time Type Information - Yes

For more details, you can refer to [wxWebViewChromium's wiki](https://github.com/wxWidgets/wxWebViewChromium/wiki/Windows-Build). 

## Building Instructions 

wxWebViewChromium supports Windows/Linux/Mac OS X platforms.
For developers who want to use webviewChromium feature, they need to link to webviewchromium library on Windows/Linux platforms(
On Mac OS X, they need to do more, see below section).

###Common Steps

1. Clone the repository by running `git clone https://github.com/hokein/wxWidgets.git` 

2. Copy the CEF3 directory to wxWidgets src directory(wxWidgets sets `<wxWidgets_src>/src/cef` as a default library).

###Windows Platform

By default, wxWidgets provides Visual Stdio 2010 project file to build wxWebViewChromium, and set Chromium Embedded Framework in `<wxWidgets_src>/cef` directory.

1. Open `build/msw/wc_vc10.sln` project file in Visual Stdio 2010, and build `wxWidgets` library.

2. Open `samples/webview_chromium/webview_chromium_vc10.vcxproj` file in Visual Stdio 2010, and build it.

3. Copy CEF3 related resources(libcef.dll, libffmpegsumo.so, locales/\*, cef.apk, devtools_resources.pak) to webview_chromium binary directory.


###Linux Platform

By default, wxWebviewChromium feature is disabled in makefile build, you should enable it through `enable-webviewchromium` option.

1. Regenerate the building files by running `bakefile_gen -f autoconf` command under `build/bakefiles` directory.

2. Run `./autogen.sh` under `<wxWidgets_src>`.

3. Run `./configure --enable-webview --enable-webviewchromium` and `make` commands.

4. Copy CEF3 related resources as above step5 in windows Platform.

5. Add `libcef.so` directory path to `LD_LIBRARY_PATH`(`export LD_LIBRARY_PATH=<libcef_directory>`),
otherwise webview_chromium will not find `libcef.so`.


###Mac OS X Platform

wxWidgets supports xcode/makefile builds, requre OS X 10.8 or above.

* Xcode builds: open `samples/webview_chromium/webview_chromium.xcodeproj`, and build `webview_chromium` sample target.

* Makefile: the same as linux platform mentioned above.


Due to the application bundle structure on OS X, wxWebviewChromium is a little complicated than win/linux platfroms.
We need an extra `helper` application for executing separate other chromim processes(renderer, plugin, etc), so they
have separated app bundles and Info.plist.

For application using webviewChromium, below are details steps, you can retrieve it in `webview_chromium.bkl`(samples/webview_chromium/webview_chromium.bkl) files:

1. Build webviewchromium library.
   * Use system tool `install_name_tool -change` to correct `Chromium Embedded Framework.framework/Chromium Embedded Framework` location.

2. Compiled/link/package the `helper` app:
   * Require `process_helper_mac.cpp`
   * Reuqire link frameworks: Chromium Embedded Framework.framework, AppKit.frameworks.
   * Reuqire app bundle configuration: Info.plist
   * Use system tool `install_name_tool -change` to correct `Chromium Embedded Framework.framework/Chromium Embedded Framework` location.

2. Compiled/link/package the `webviewchromium` app:
   * Require `webview.cpp`
   * Reuqire link frameworks: Chromium Embedded Framework.framework, AppKit.frameworks.
   * Reuqire app bundle configuration: Info.plist
   * Use system tool `install_name_tool -change` to correct `Chromium Embedded Framework.framework/Chromium Embedded Framework` location.

3. Create a `Contents/Frameworks` directory in `webviewchromium.app` bundle and copy `helper.app`, `webviewchromium.dylib` and `Chromium Embedded Framework` in it.

4. Use `samples/webview_chromium/mac/tools/make_more_helper.sh` to make sub-process helper app bundles based on `helper` app.


Below is the wxWebviewChromium sample app bundle directory structure:

```
webview_chromium.app
`-- Contents
    |-- Frameworks
    |   |-- Chromium Embedded Framework.framework       <= CEF framework
    |   |   |-- Chromium Embedded Framework
    |   |   |-- Libraries
    |   |   |   `-- ffmpegsumo.so
    |   |   `-- Resources\
    |   |-- libwx_osx_cocoau_webviewchromium-3.1.dylib  <= wxWebviewChromium library supports webviewChromium backend.
    |   |-- webview_chromium Helper EH.app              <= helper app
    |   |   `-- Contents
    |   |       |-- Info.plist
    |   |       |-- MacOS
    |   |       |   `-- webview_chromium Helper EH
    |   |       |-- PkgInfo
    |   |       `-- Resources
    |   |           `-- wxmac.icns
    |   |-- webview_chromium Helper NP.app              <= helper app
    |   |   `-- Contents
    |   |       |-- Info.plist
    |   |       |-- MacOS
    |   |       |   `-- webview_chromium Helper NP
    |   |       |-- PkgInfo
    |   |       `-- Resources
    |   |           `-- wxmac.icns
    |   `-- webview_chromium Helper.app                 <= helper app
    |       `-- Contents
    |           |-- Info.plist
    |           |-- MacOS
    |           |   `-- webview_chromium Helper
    |           |-- PkgInfo
    |           `-- Resources
    |               `-- wxmac.icns
    |-- Info.plist
    |-- MacOS
    |   `-- webview_chromium                            <= webviewchromium sample executable
    |-- PkgInfo
    `-- Resources
        `-- wxmac.icns                                  <= resources.
```

##Notes

wxWidgets will set `wxWidgets_src/src/cef` directory as default CEF path. If you want to specify your custom CEF path, please follow the steps:

1. Specify your CEF path to `CEF_INCLUDE_DIR` variable in `build/bakefiles/config.bkl` file.

2. Regenerate the building files by running `bakefile_gen -f <formats>`.

[1]: http://code.google.com/p/chromiumembedded/
