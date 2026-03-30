#include "main.h"
namespace {
constexpr const char kOpenUrls[] = R"json(
    ["https://baidu.com","https://bing.com","https://myip.ipipv.com"])json";
constexpr const char kOpenCookies[] = R"json(
[
    {
        "domain": ".bing.com",
        "expirationDate": 1779087533,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_uetmsclkid",
        "path": "/",
        "sameSite": null,
        "secure": false,
        "session": false,
        "storeId": null,
        "value": "_ueta4d4770aaf1c105213e31e98339d6bf5"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1779930195.508655,
        "hostOnly": false,
        "httpOnly": false,
        "name": "SRCHUSR",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "DOB=20250215&T=1743499065000&DS=1&POEX=W"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": true,
        "name": "_Rwho",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "u=d&ts=2026-03-27"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1774686562.282214,
        "hostOnly": false,
        "httpOnly": false,
        "name": "GC",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "Phg5rQuVsIcBdQd0TFoV2Ny12D9EerbC4fgDbLqyEVRWtqlAm63nSKGL2-zcXOgyJ105CHuKTdMKn2AfSj2V3A"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1805246936.974737,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_TTSS_OUT",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "hist=WyJ6aC1IYW5zIl0="
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1809160163.138901,
        "hostOnly": false,
        "httpOnly": false,
        "name": "SRCHHPGUSR",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "SRCHLANG=zh-Hans&PV=19.0.0&PREFCOL=0&BRW=XW&BRH=M&CW=1456&CH=837&SCW=1441&SCH=837&DPR=1.5&UTC=480&PRVCW=602&PRVCH=837&HV=1774600161&HVE=CfDJ8HAK7eZCYw5BifHFeUHnkJGXIhwKna3u9SKJgcTLTckLGqlEgnY8DWjov1X0FoR6VGaCYADQm5nnZfHuLvj8FDd_Ib_uEompBSQnU7zFYVFDFLj8W4KWj4oHRkgEQAa16CqVIxcyS6dgQFsN-C8o02KFuGXk-v3C2tq9ODNFrXMs1C-5iPnDsXACi89RkWoFzQ&BZA=0&B=0&EXLTT=31&AV=14&ADV=14&RB=0&MB=0&IG=3A97CC94133B40D0887B84F0048467F0"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1790497440.77731,
        "hostOnly": false,
        "httpOnly": false,
        "name": "ANON",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "A=D6257A6A6A623E7D5FD80140FFFFFFFF"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": false,
        "name": "_SS",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "SID=23C4F91F49FF64551B4FEE364899653E&R=15903&RB=15903&GB=0&RG=0&RP=15903"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": false,
        "name": "ipv6",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "hit=1774603761294&t=4"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1775809440.777087,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_U",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "1cY0yEA2Czlg_GNamDnk2ZtzgMsPccJ4RKfoKlx_t-Fed9RXJzx7UnWkyH4E2cHV4X6T9zrcuZfjo0qDAhLGocy8XvgSQJvZcdG_DzyBJD0ZkLphIQhJseNBbgmsFWYWYW0jGWEoxwHtlwYP88ihhCJ60fVHBIFS9vCah0dJs7DDRnXkjuBwgm3eHomgiXmdA-v1vP5s-7fxMfswlpKiNfl-OM7EeCfS1e58KJeqsOsU"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1804552743.011825,
        "hostOnly": false,
        "httpOnly": false,
        "name": "SRCHD",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "AF=NOFORM"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1783725265.257717,
        "hostOnly": false,
        "httpOnly": false,
        "name": "NAP",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "V=1.9&E=1fc8&C=PoMjlIMyChRgF88GgjJ2hICinKXfMmOdxCwENxDZolsz9okKHeLmtQ&W=1"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1809160161.03269,
        "hostOnly": false,
        "httpOnly": false,
        "name": "BFBUSR",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "BFBHP=0"
    },
    {
        "domain": "cn.bing.com",
        "expirationDate": 1775834960.747737,
        "hostOnly": true,
        "httpOnly": true,
        "name": "__Host-XY",
        "path": "/",
        "sameSite": "lax",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "CfDJ8HAK7eZCYw5BifHFeUHnkJGov_UGIQtolBymJdMj4Pv6Rcpn3o4ppF6B_vTb9NMU0uEyofDHcgEGJ6paEmXLiS1twfPY8Bs0khqtZcS8ai_FUgZ3XAxiaX4N1V_CgH5TBU77-R9Khw9IICB7V-1a6lART4h5wSnhWZ8O74vMMUmY3lG-rs4zb2osabl3CoPjysjX7zMOh9a-BvaEFtc3PaDI2Vz6Nouxd1a8vtzZRsTSJPZ6UU7ZIZ3zmIkioupIcsoWoCaMZknDpwv1HqRMZdc"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1806136162.282512,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_RwBf",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "r=1&mta=0&ispd=0&rc=15903&rb=15903&rg=0&pc=15903&mtu=0&rbb=0.0&clo=0&v=2&l=2026-03-27T07:00:00.0000000Z&lft=0001-01-01T00:00:00.0000000&aof=0&ard=0001-01-01T00:00:00.0000000&rwdbt=-62135539200&rwflt=1750469982&rwaul2=0&g=newLevel1&o=0&p=MULTIGENREWARDSCMACQ202205&c=ML2357&t=321&s=2022-12-26T02:54:45.8677517+00:00&ts=2026-03-27T08:29:19.4859823+00:00&rwred=0&wls=0&wlb=0&wle=0&ccp=2&cpt=0&lka=0&lkt=0&aad=0&TH=&cid=180&gb=2025w17_c&e=CEjy8qUSyEkCPMlDRrg_B-ZlgIw0gs9fcg3r8WBE8anotGW1fGXBV_dA1Ftavw25NSczrwoLw9NacqYWMJNc9OBwGcJC9PIKqm7xSnzPPik&A="
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1808189904.38016,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_UR",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "QS=0&TQS=0&cdxcls=0&Pn=3"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": true,
        "name": "_EDGE_S",
        "path": "/",
        "sameSite": null,
        "secure": false,
        "session": true,
        "storeId": null,
        "value": "SID=23C4F91F49FF64551B4FEE364899653E"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": true,
        "name": "_C_ETH",
        "path": "/",
        "sameSite": null,
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "1"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1784940324.463313,
        "hostOnly": false,
        "httpOnly": false,
        "name": "imgv",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "lodlg=1&gts=20250620"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1809144869.997242,
        "hostOnly": false,
        "httpOnly": true,
        "name": "USRLOC",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "HS=1&ELOC=LAT=30.246747970581055|LON=120.2314453125|N=%E8%90%A7%E5%B1%B1%E5%8C%BA%EF%BC%8C%E6%B5%99%E6%B1%9F%E7%9C%81|ELT=4|&BID=MjYwMzI3MTYyNzE1XzNkZDZmYTczZTNkNTdhNzA1NzVkYzA2NWU1NThkMzRhM2M4NzVlY2I4NjY5ODgyMjMwNTc0NWRkY2U5YTJiNmM="
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1774686240.775778,
        "hostOnly": false,
        "httpOnly": true,
        "name": ".MSA.Auth",
        "path": "/",
        "sameSite": "lax",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "CfDJ8HAK7eZCYw5BifHFeUHnkJEuowA1xl7wyLy-E5QqMJG-ZBaBsBjoFPe0ikQxTnsqfiHcVLir3sQq-D7nBFbwO7Y7Qbefl0-OZ-gIKzIktdsYvmUatU0hkCyzco5WoKmGBlA9U-l-IaatKdJgCFU4t83Q1a0vrsS-7x5XwBFKXocgmmpVygq3l2BYPE04-sOSsk9WIJTEIStQLfCEVTv4ceoFXtqCOa4IhNVxAqOq0TUgNrwnE88p0A9vTOVYSkyRMr4_iFrfbeR1wM4iVYmVctBRACDWAxdwVqPJtZ9OSO35SXsIg-ECUQq2JiOQzTwkQJOnhrNGwDvzdBReQc5tBfTZaw5Of0g1nQOwsX-YlJBBheutV5--22znMB5r8-BPcg"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1789380023,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_clck",
        "path": "/",
        "sameSite": null,
        "secure": false,
        "session": false,
        "storeId": null,
        "value": "1oajv5l%5E2%5Efzb%5E1%5E1425"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1809160162.712756,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_HPVN",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "CS=eyJQbiI6eyJDbiI6NDYxLCJTdCI6MywiUXMiOjAsIlByb2QiOiJQIn0sIlNjIjp7IkNuIjo0NjEsIlN0IjowLCJRcyI6MCwiUHJvZCI6IkgifSwiUXoiOnsiQ24iOjQ2MSwiU3QiOjAsIlFzIjowLCJQcm9kIjoiVCJ9LCJBcCI6dHJ1ZSwiTXV0ZSI6dHJ1ZSwiTGFkIjoiMjAyNi0wMy0yN1QwMDowMDowMFoiLCJJb3RkIjowLCJHd2IiOjAsIlRucyI6MCwiRGZ0IjpudWxsLCJNdnMiOjAsIkZsdCI6MCwiSW1wIjoyMTE0LCJUb2JuIjowfQ=="
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1805246936.967438,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_tarLang",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "default=zh-Hans"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1805246936.973217,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_TTSS_IN",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "hist=WyJlbiIsIml0IiwiYXV0by1kZXRlY3QiXQ==&isADRU=0"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1805007533,
        "hostOnly": false,
        "httpOnly": false,
        "name": "_uetvid",
        "path": "/",
        "sameSite": null,
        "secure": false,
        "session": false,
        "storeId": null,
        "value": "1ebd8e600bce11f1beb3f76aba6f7893"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1806152071.279688,
        "hostOnly": false,
        "httpOnly": false,
        "name": "MMCASM",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "ID=6C594A89B95041489A31DE4B3AD4C4AE"
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1808258734.615917,
        "hostOnly": false,
        "httpOnly": false,
        "name": "MUID",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "159EFD41A306620A0F04EE93A2D46379"
    },
    {
        "domain": "cn.bing.com",
        "expirationDate": 1808296164.356873,
        "hostOnly": true,
        "httpOnly": true,
        "name": "MUIDB",
        "path": "/",
        "sameSite": null,
        "secure": false,
        "session": false,
        "storeId": null,
        "value": "159EFD41A306620A0F04EE93A2D46379"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": false,
        "name": "SNRHOP",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "I=&TS="
    },
    {
        "domain": ".bing.com",
        "expirationDate": 1804552743.011856,
        "hostOnly": false,
        "httpOnly": false,
        "name": "SRCHUID",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": false,
        "storeId": null,
        "value": "V=2&GUID=47A67F18C796453BBDA2C6FE3214A7BC&dmnchg=1"
    },
    {
        "domain": ".bing.com",
        "hostOnly": false,
        "httpOnly": false,
        "name": "WLS",
        "path": "/",
        "sameSite": "no_restriction",
        "secure": true,
        "session": true,
        "storeId": null,
        "value": "C=2a01facd3e84ef6b&N=%e7%94%9f"
    }
]
)json";
constexpr const char kOpenClearCookies[] =
    R"json(
[{"domain":"","expirationDate":0.0,"hostOnly":false,"httpOnly":false,"name":"BAIDUID_BFESS","path":"/","sameSite":"no_restriction","secure":true,"session":false,"storeId":null,"value":""}])json";
} // namespace

int main(int argc, char **argv) {
  SDKWrapper sdk;
  if (sdk.Ready()) {
    // Ensure wide output supports the user's locale (for Chinese output).
    try {
      std::locale::global(std::locale(""));
    } catch (...) {
      // Fallback: keep classic "C" locale if system locale not available.
    }
    std::wcout.imbue(std::locale());
    std::wcout << L"BroSDK is ready." << std::endl;
    std::wcout << LR"(
Demo (自动化测试模式)

- 描述: 自动化测试，快速验证接口功能并生成请求数据，适用于开发/调试阶段。
- 初始化:
  - `sdk_init` 或 `init` : 同步初始化 SDK
  - `sdk_init_async`     : 异步初始化
  - `sdk_init_webapi`    : 启动内置 HTTP/WebSocket 服务（示例端口 65535）
- 浏览器控制:
  - `sdk_browser_open` 或 `open`  : 打开一组浏览器环境（自动生成请求）
  - `sdk_browser_close` 或 `close`: 关闭当前环境集合
  - `auto` : 启动自动化示例（会反复 open/close）
  - `q` : 退出 DEMO

示例: 在命令行输入 `open` 会创建环境并打开浏览器，随后观察回调通知，输入 `close` 会关闭。
)" << std::endl;

    sdk.Init();

    //std::string sdkInfo;
    //sdk.GetInfo(sdkInfo);
  }
  // 127: 2036982559262380032
  // 131: 2033395379034132480
  // 134: 2036982553230970880
  // 138: 2033435302650974208
  // 140: 2036982563230191616
  // 141: 2037697880264806400
  // edge: 2036387200416878592
  // chrome: 2036387214597820416
  std::set<uint64_t> envIds{
#if !defined(IS_APPLE)
      //2036982553230970880, 2036387200416878592, 
      //2036387214597820416,
      //2038459365622550528
      2038511377223520256,
#else
      2036982553230970880,
      2036982559262380032,
      2036982563230191616,
#endif
  };
  Utils::MainProc([&sdk, &envIds](const std::string &input, bool &exit) {
    if (input == "q") {
      exit = true;
    } else if (input == "sdk_init" || input == "init") {
      sdk.Init();
    } else if (input == "sdk_info" || input == "info") {
      std::string info;
      sdk.GetInfo(info);
      std::cout << info << std::endl;
    } else if (input == "sdk_browser_open" || input == "open") {
      std::string reqBody;
      sdk.GenOpenBrwReqBody(envIds, {}, kOpenUrls, /*kOpenCookies*/ "",
                            reqBody);
      sdk.BrowserOpen(reqBody.c_str(), reqBody.size());
      //  const auto genOpenReq = GenOpenBrwReqBody();
      //  sdk.BrowserOpen(genOpenReq.c_str(), genOpenReq.size());
    } else if (input == "sdk_browser_close" || input == "close") {
      std::string reqBody;
      sdk.GenCloseBrwReqBody(envIds, reqBody);
      sdk.BrowserClose(reqBody.c_str(), reqBody.size());
    } else if (input == "install") {
      const std::string reqBody = R"({
	"note-1": "major、minor、build、patch",
	"note-2": "const BROWSER_FIREFOX = 'firefox'; const BROWSER_CHROMIUM = 'chromium'; const browserType = BROWSER_CHROMIUM;",
	"browserType": "chromium",
	"cores": [
		{
			"major": 138
		},
		{
			"major": 127
		}
	]
})";
      sdk.BrowserInstall(reqBody);
    } else if (input == "auto") {
      sdk.StartAutoTest();
    } else if (input == "page") {
      sdk.BrowserEnvPage("{}");
    } 
    else if (input == "update") {
      sdk.BrowserEnvUpdate("{}");
    } 
    else if (input == "create") {
      // sdk.BrowserEnvCreate("138");
      // socks5://faze06a2:faze06a2@49.7.133.120:2018
      /*
        "proxy": "socks5h://pgkwp4hyzvsp:zvvgbcx8n5n3@202.160.80.63:5206",
"bridgeProxy": "socks5://pgkwp4hyzvsp:zvvgbcx8n5n3@hsgzhk6.ipipv.com:7927",
      */
      // socks5://1A2qYpA154641428A32880:CZehJuct59P4@154.64.142.8:32880
     //socks5h://1A2qYpA154641428A32880:CZehJuct59P4@hsgzrf10.ipipv.com:6383
      auto id_1 = sdk.BrowserEnvCreate(
          "138",
          "socks5://1A2qYpA154641428A32880:CZehJuct59P4@154.64.142.8:32880",
          "socks5h://"
          "1A2qYpA154641428A32880:CZehJuct59P4@hsgzrf10.ipipv.com:6383");
      auto id0 = sdk.BrowserEnvCreate(
          "134", "socks5://pgkwp4hyzvsp:zvvgbcx8n5n3@202.160.80.63:5206",
          "socks5h://pgkwp4hyzvsp:zvvgbcx8n5n3@hsgzhk6.ipipv.com:7927");
      #if  0
      auto id1 = sdk.BrowserEnvCreate(
          "127", "socks5://pgkwp4hyzvsp:zvvgbcx8n5n3@202.160.80.63:5206",
          "socks5h://pgkwp4hyzvsp:zvvgbcx8n5n3@hsgzhk6.ipipv.com:7927");
      auto id2 = sdk.BrowserEnvCreate(
          "140", "socks5://pgkwp4hyzvsp:zvvgbcx8n5n3@202.160.80.63:5206",
          "socks5h://pgkwp4hyzvsp:zvvgbcx8n5n3@hsgzhk6.ipipv.com:7927");
      auto id3 = sdk.BrowserEnvCreate(
          "141", "socks5://pgkwp4hyzvsp:zvvgbcx8n5n3@202.160.80.63:5206",
          "socks5h://pgkwp4hyzvsp:zvvgbcx8n5n3@hsgzhk6.ipipv.com:7927");
      #endif
      //if (id1 && id2) {
      //  int a = 0;
      //}
    } else if (input == "clear") {
      std::string reqBody;
      sdk.GenOpenBrwReqBody(envIds, {}, kOpenUrls, kOpenClearCookies, reqBody);
      sdk.BrowserOpen(reqBody.c_str(), reqBody.size());
    } else {
      std::cout << "Unknown command: " << input << std::endl;
    }
  });

  sdk.Shutdown();
  return 0;
}
