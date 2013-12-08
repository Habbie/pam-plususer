pam-plususer
============

This PAM module splits a username on the first +, resetting the PAM username
to the first part and sticking the rest in the PAM_PLUSUSER environment
variable.

Code is mostly untested as I lost interest when I found out about
https://bugzilla.mindrot.org/show_bug.cgi?id=1215 the hard way.

To see the module in action, stick this in /etc/pam.d/check, steal
`/usr/share/doc/libpam0g-dev/examples/check_user.c` from the Debian libpam
package,  and run `check_user peter+foo`, for example.

````
auth       optional     /home/peter/pam-plususer/pam_plususer.so
auth       required     pam_unix.so
account    required     pam_permit.so
session	required			pam_permit.so
````