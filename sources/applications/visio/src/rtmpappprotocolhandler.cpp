/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "rtmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
using namespace app_visio;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

  this->hosts_allowed=configuration["hosts_allowed"];
  FINEST("Hosts allowed:\n%s", STR(this->hosts_allowed.ToString()));

}

bool RTMPAppProtocolHandler::CheckHost(string host){

  if(this->hosts_allowed==V_NULL){return true;}

  bool test=false;
  
  FOR_MAP(this->hosts_allowed, string, Variant, i) {
    Variant &target = MAP_VAL(i);
    if(((string) target)==host){test=true;}
  }
  return test;

}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

bool RTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
						  Variant &request) {
 
  string functionName = M_INVOKE_FUNCTION(request);
  FINEST("Function name:\n%s", STR(functionName));
    return BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(pFrom, request);
  
}

bool RTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom,
						  Variant &request) {
  //0. Dump the request on console, just to see its structure
  FINEST("Initial request:\n%s", STR(request.ToString()));
  //1. Get the connect params from the connect invoke
  Variant connectParams = M_INVOKE_PARAM(request, 0);
 
  //2. This should be a key-value map
  if (connectParams != V_MAP) {
    FATAL("Incorrect invoke params:\n%s", STR(request.ToString()));
    return false;
  }
 
  //3. Let's extract few values. Make sure we extract them using non-case-sensitive keys
  Variant tcUrl = connectParams.GetValue(RM_INVOKE_PARAMS_CONNECT_TCURL, false);
 
  //If you are sure about case-sensitive settings, you can extract it directly like this
  Variant swfUrl = connectParams[RM_INVOKE_PARAMS_CONNECT_SWFURL];
  //Variant tcUrl = connectParams[RM_INVOKE_PARAMS_CONNECT_TCURL];
  Variant pageUrl = connectParams.GetValue(RM_INVOKE_PARAMS_CONNECT_PAGEURL,false);

//4. Do some validation on them.
 
   // validate pageURL for NULL values before tcURl since we are gongin to validate Uri.host from pageUrl
   if (pageUrl != V_STRING) {
     FATAL("Incorrect "RM_INVOKE_PARAMS_CONNECT_PAGEURL": %s", STR(request.ToString()));
     return false;
   }
 

  URI uri;
  if (!URI::FromString(pageUrl, true, uri)) {
    FATAL("Unable to parse the uri %s", STR(request.ToString()));
    return false;
  }else{

    FINEST("URI :\n%s", STR(uri.ToString()));
  }
    if (!this->CheckHost(uri.GetValue("host",false))) {
      FATAL("Connection not allowed from : %s", STR(uri.GetValue("host",false)));
    return false;
    }else{
      FINEST("Connection allowed from : %s", STR(uri.GetValue("host",false)));
    }


//5. If everything looks right, continue default processing :)
  return BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request);
}
