#include <cerrno>
#include <cstdio>
#include <lua.hpp>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

#if __has_include(<syslog.h>)
#include <syslog.h>
#else
#define LOG_INFO (1)
#define LOG_ERR (2)
#define syslog(a, ...) fprintf((a) == LOG_INFO ? stdout : stderr, __VA_ARGS__)
#endif

extern "C" {
static int simple_conversation(int num_msg, const struct pam_message **msg,
                               struct pam_response **resp, void *appdata_ptr);
static int auth_current_user(lua_State *L);
int luaopen_liblua_pam(lua_State *L);
}

static int simple_conversation(int num_msg, const struct pam_message **msg,
                               struct pam_response **resp, void *appdata_ptr) {
    if (num_msg == 0) {
        return PAM_ABORT;
    }

    /* PAM expects an array of responses, one for each message */
    if ((*resp = (struct pam_response *)calloc(
             num_msg, sizeof(struct pam_response))) == NULL) {
        syslog(LOG_ERR, "calloc failed: %s\n", strerror(errno));
        return PAM_ABORT;
    }

    for (int c = 0; c < num_msg; c++) {
        /* return code is currently not used but should be set to zero */
        resp[c]->resp_retcode = 0;

        if (msg[c]->msg_style != PAM_PROMPT_ECHO_OFF &&
            msg[c]->msg_style != PAM_PROMPT_ECHO_ON) {
            continue;
        }

        if ((resp[c]->resp = strdup((const char *)appdata_ptr)) == NULL) {
            syslog(LOG_ERR, "strdup failed: %s\n", strerror(errno));
            return PAM_ABORT;
        }
    }

    return PAM_SUCCESS;
}

static bool __auth_current_user(const char *pass) {
    int retval;

    uid_t uid = getuid();
    passwd *pw = getpwuid(uid);
    const char *user = pw->pw_name;
    bool is_auth = false;

    pam_handle_t *pamh = NULL;

    struct pam_conv conv = {simple_conversation, (void *)pass};

    do {
        if ((retval = pam_start("login", user, &conv, &pamh)) != PAM_SUCCESS) {
            syslog(LOG_ERR, "pam_start failed: %s\n",
                   pam_strerror(pamh, retval));
            break;
        }

        if ((retval = pam_authenticate(pamh, 0)) != PAM_SUCCESS) {
            syslog(LOG_ERR, "pam_authenticate failed: %s\n",
                   pam_strerror(pamh, retval));
            break;
        }

        is_auth = true;
        syslog(LOG_INFO, "Authenticated\n");

        if ((retval = pam_close_session(pamh, 0)) != PAM_SUCCESS) {
            syslog(LOG_ERR, "pam_close_session failed: %s\n",
                   pam_strerror(pamh, retval));
        }
    } while (0);

    if (pamh) {
        if ((pam_end(pamh, retval)) != PAM_SUCCESS) {
            syslog(LOG_ERR, "pam_end failed: %s\n", pam_strerror(pamh, retval));
        }
    }

    return is_auth;
}

static int auth_current_user(lua_State *L) {
    bool ret = __auth_current_user(luaL_checkstring(L, -1));

    /* This is where we have been authorized or not. */
    lua_pushboolean(L, ret);
    return 1;
}

static const struct luaL_Reg lua_pam[] = {
    {"auth_current_user", auth_current_user}, {NULL, NULL} /* sentinel */
};

int luaopen_liblua_pam(lua_State *L) {
    luaL_newlib(L, lua_pam);
    return 1;
}

#ifdef TEST
int main() {
    const char *pass = "wrong password";

    int ret = __auth_current_user(pass);

    printf("result: %d\n", ret);

    return 0;
}
#endif
