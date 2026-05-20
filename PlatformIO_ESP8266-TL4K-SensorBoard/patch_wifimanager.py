Import("env")
import os

WIFI_MANAGER_CPP = os.path.join(
    env.subst("$PROJECT_LIBDEPS_DIR"),
    env.subst("$PIOENV"),
    "WiFiManager",
    "WiFiManager.cpp",
)

OLD_HTTPSEND = 'void WiFiManager::HTTPSend(const String &content){\n  server->send(200, FPSTR(HTTP_HEAD_CT), content);\n}'

NEW_HTTPSEND = 'void WiFiManager::HTTPSend(const String &content){\n  server->setContentLength(CONTENT_LENGTH_UNKNOWN);\n  server->send(200, FPSTR(HTTP_HEAD_CT), "");\n  server->sendContent(content);\n  server->sendContent("");\n}'

def patch_wifimanager(source, target, env):
    if not os.path.isfile(WIFI_MANAGER_CPP):
        print("PATCH: WiFiManager.cpp not found at %s, skipping" % WIFI_MANAGER_CPP)
        return

    with open(WIFI_MANAGER_CPP, "r", encoding="utf-8") as f:
        content = f.read()

    if OLD_HTTPSEND in content:
        content = content.replace(OLD_HTTPSEND, NEW_HTTPSEND)
        with open(WIFI_MANAGER_CPP, "w", encoding="utf-8") as f:
            f.write(content)
        print("PATCH: WiFiManager HTTPSend -> chunked encoding applied")
    elif NEW_HTTPSEND in content:
        print("PATCH: WiFiManager already patched, skipping")
    else:
        print("PATCH: WARNING - could not find HTTPSend pattern in WiFiManager.cpp")

env.AddPreAction("$BUILD_DIR/libbe4/libWiFiManager.a", patch_wifimanager)
