This repo is a port of the Chrome legacy browser extension and plug-in to Firefox

---

# Goal
Bounce the navigation back to Chrome whenever the user is trying to open a web
page that is meant to be opened in Chrome. It is meant to be a companion to the
Chrome Legacy Browser Support Extension to allow for seamless switching between
Chrome and IE.


# Implementation
Internet Explorer:
The plug-in is implemented as an Internet Explorer Add-on, and is packaged in a
Windows installer. The installer adds a DLL to the user's machine and registers
the plug-in in the browser (Tools -> Manage Add-ons). Once installed, the Add-on
monitors navigation requests in IE and checks if the url is any of the
whitelisted ones to be opened in IE. If not it will close the tab that started
the navigation and reopen the url in Chrome.

# policies.json

```
{
  "policies": {
    "3rdparty" {
      "Extensions": {
        "legacy-browser-support@mozilla.org" {
          "alternative_browser_path": "${ie}",
          "alternative_browser_arguments": "${url}",
          "firefox_path": "${firefox}",
          "firefox_arguments": "${url}",
          "url_list": ["example.com"],
          "url_greylist": ["example.org"],
          "keep_last_firefox_tab": true | false,
          "show_transition_screen": 0-60
          "use_ie_site_list": true | false,         
        }
      }
    }
  }
}
```

`alternative_browser_path` - Allows you to specify the program to be used as an alternative browser.

If enabled you can either specify an absolute path in the policy or use one of the following variables:

${ie} - The default install location for Internet Explorer

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\IEXPLORE.EXE is used to pick the location of the browser in this case.

${chrome} - The default install location for Chrome

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\Chrome.EXE is used to pick the location of the browser in this case.

${safari} - The default install location for Safari

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\SAFARI.EXE is used to pick the location of the browser in this case.

If the policy is not set or left empty the default alternative browser will be used which is Internet Explorer determined as if the ${ie} value is used.

`alternative_browser_arguments` - Allows you to specify the arguments to be passed to the executable of the alternative browser.

If the policy is enabled the provided parameters will be used when the alternative browser is invoked.

You can also use the special placeholder ${url} to specify where the url should appear in the command line. E.g. "--url=${url} --kiosk".

You don't have to specify the placeholder if it should be appended to the end or if it should be the only argument you can leave that policy empty.

If the policy is not set or left empty only the url will be passed as a parameter to the browser.

`firefox_path` - Allows you to specify the executable of Firefox to be launched when returning from the alternative browser(*).

When the policy is enabled you can either specify an absolute path in the policy or use the following variable:

${firefox} - The default install location for Firefox

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\FIREFOX.EXE is used to pick the location of the browser in this case.

If the policy is not set or left empty the default installation of Firefox will be used as if the ${firefox} value has been used.

*: Presently only Internet Explorer supports returning to Firefox automatically.

`firefox_arguments` - Allows you to specify the arguments to be passed to the Firefox when it is executed.

If the policy is enabled the provided paramters will be used when Firefox is invoked.

You can also use the special placeholder ${url} to specify where the url should appear in the command line. E.g. "--url=${url} --kiosk".

You don't have to specify the placeholder if it should be appended to the end or if it should be the only argument you can leave that policy empty.

If the policy is not specified only the url will be passed as a parameter to the browser.

`url_list` - Allows you to specify a list of host domain names to be opened in the Alternative browser.  

If the policy is enabled you have to provide a list of filters which will trigger the transition to the alternative browser. Every entry should be one of the following four types:

Host-name part: Either complete domain names like "www.example.com" should be specified or parts of them like "example.com" or even "example". Wildcards are not supported yet.  

URL prefix: Only proper URL prefixes are matched complete with protocol and port if needed. E.g. "http://login.example.com" or "https://www.example.com:8080/login/".  

Negative entry: Starts with "!" and continues as host-name part or URL prefix as described above. Negative entries are always opened in Firefox. E.g. "!example.com" or "!file:///c:/localapp/".  

Wildcard entry: Contains only a single "*" character. Matches any URL. Intended to be used together with negative entries, if most URLs should be open in the alternative browser and only a small selection of URLs should be opened in Firefox.  

Negative entries have higher priorities than positive entries which allows to whitelist large portions of a domain while saving smaller sections to be opened in Firefox.

The wildcard entry if present is applied only after all other rules have been checked.

The following protocols are monitored for redirecting: http:, https:.

If not specified or left empty, no transition to the alternative browser will be triggered.

`url_greylist` - Allows you to specify a list of host domain names to be opened in either browser.

When this policy is enabled, the domains in this list will be available in both browsers and will not trigger a transition either way. A possible use case are any authentication domains that are shared between new and legacy apps in this list.

Host-name parts: Either complete domain names like "www.example.com" should be specified or parts of them like "example.com: or even "example". Wildcards are not supported yet.

URL prefixes: Only proper URL prefixes are matched complete with protocol and port if needed. E.g. "http://login.example.com" or "https://www.example.com:8080/login/".

If not specified or left empty, any domain not in the "Hosts to Open In the Alternative Browser" list will trigger a transition back to Firefox(*).

*: Presently only Internet Explorer supports returning to Firefox automatically.

`keep_last_firefox_tab` - Allows you to specify what happens when the last open tab in Firefox is being redirected to the alternative browser.

When this policy is true or not set the last tab will be left open and point to the new tab page after the transition. If it is set to false then the last tab will be closed which will lead to closing Firefox if there are no other Firefox windows open.

`show_transition_screen` - Allows you to specify how long the transition message in the tab will be visible in Firefox before transitioning to the other browser. This helps visualize the transition. The policy is specified in seconds.

If it is not set then the tab will be closed immediately when the transition can be completed.

`use_ie_site_list` - When enabled, transitions between browsers will also be triggered by entries in the SiteList policy for Internet Explorer as defined in (https://technet.microsoft.com/itpro/internet-explorer/ie11-deploy-guide/turn-on-enterprise-mode-and-use-a-site-list).

If it is not set then only the urls in the extension policy will be used.
