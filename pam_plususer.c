#define PAM_SM_AUTH

#include <security/pam_modules.h>
#include <stdio.h>
#include <syslog.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#define UNUSED __attribute__ ((unused))

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh,
                                   int flags,
                                   int argc,
                                   const char **argv)
{
    const void *user;
    char *realuser;
    char *plus;
    char *split;
    int retval;

    retval = pam_get_item(pamh, PAM_USER, &user);
    if (retval != PAM_SUCCESS || user == NULL || *(const char *)user == '\0') {
        pam_syslog(pamh, LOG_NOTICE, "user unknown");
        return PAM_USER_UNKNOWN;
    }

    fprintf(stderr, "user=%s\n", (const char *) user);

    realuser = plus = NULL;
    split = strchr(user, '+');
    if(split) {
        realuser = strndup(user, split-(const char *)user);
        retval = asprintf(&plus, "PAM_PLUSUSER=%s", (const char *)split+1);
        fprintf(stderr, "%d\n", retval);
    } else {
        return PAM_SUCCESS;
    }

    fprintf(stderr, "realuser=%s\n", realuser);
    fprintf(stderr, "plus=%s\n", plus);

    retval = pam_set_item(pamh, PAM_USER, (const void *)realuser);
    if (retval != PAM_SUCCESS) {
        free(realuser);
        free(plus);

        pam_syslog(pamh, LOG_ERR, "pam_set_item(.., PAM_USER, ..) failed");
        return PAM_USER_UNKNOWN;
    }

    retval = pam_putenv(pamh, plus);
    if (retval != PAM_SUCCESS || user == NULL) {
        free(realuser);
        free(plus);

        pam_syslog(pamh, LOG_ERR, "pam_putenv failed");
        return PAM_USER_UNKNOWN;
    }

    free(realuser);
    free(plus);

    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh UNUSED, int flags UNUSED,
                int argc UNUSED, const char **argv UNUSED)
{
  return PAM_IGNORE;
}
