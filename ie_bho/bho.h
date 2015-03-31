// Copyright 2013 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <shlguid.h>
#include <exdispid.h>
#include <shlobj.h>

#include "core/browser_switcher_core.h"
#include "ie_bho/ie_bho.h"  // Generated by the MIDL compiler.
#include "ie_bho/resource.h"

// Once loaded, this BHO ensures that any url that is not meant for IE is opened
// in the default browser.
class ATL_NO_VTABLE CBrowserSwitcherBHO
    : public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
      public ATL::CComCoClass<CBrowserSwitcherBHO, &CLSID_BrowserSwitcherBHO>,
      public ATL::IObjectWithSiteImpl<CBrowserSwitcherBHO>,
      public ATL::IDispatchImpl<IBrowserSwitcherBHO, &IID_IBrowserSwitcherBHO,
                                &LIBID_BrowserSwitcherLib, 1, 0>,
      public ATL::IDispEventImpl<1, CBrowserSwitcherBHO,
                                 &DIID_DWebBrowserEvents2,
                                 &LIBID_SHDocVw, 1, 1> {
 public:
  CBrowserSwitcherBHO() { }

  STDMETHOD(SetSite)(IUnknown *site);

  DECLARE_REGISTRY_RESOURCEID(IDR_BROWSERSWITCHERBHO)

  BEGIN_COM_MAP(CBrowserSwitcherBHO)
    COM_INTERFACE_ENTRY(IBrowserSwitcherBHO)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IObjectWithSite)
  END_COM_MAP()

  BEGIN_SINK_MAP(CBrowserSwitcherBHO)
    SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2,
                  DISPID_BEFORENAVIGATE2, BeforeNavigate)
  END_SINK_MAP()

  void STDMETHODCALLTYPE BeforeNavigate(IDispatch *disp,
                                        VARIANT *url,
                                        VARIANT *flags,
                                        VARIANT *target_frame_name,
                                        VARIANT *post_data,
                                        VARIANT *headers,
                                        VARIANT_BOOL *cancel);
 private:
  bool CheckUrl(LPOLESTR url, bool cancel);

  ATL::CComPtr<IWebBrowser2> web_browser_;
  bool advised_;

  BrowserSwitcherCore browser_switcher_;
};

OBJECT_ENTRY_AUTO(__uuidof(BrowserSwitcherBHO), CBrowserSwitcherBHO)
