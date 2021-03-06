sudo apt install autoconf -y

mkdir src

cat <<'EOF' >> src/main.c
#include <config.h>
#include <stdio.h>

int main (void)
{
  puts ("Hello World!");
  puts ("This is " PACKAGE_STRING ".");
  return 0;
}
EOF


cat <<'EOF' >> src/Makefile.am
bin_PROGRAMS = hello
hello_SOURCES = main.c
EOF


cat <<'EOF' >> README
This is a demonstration package for GNU Automake.
Type 'info Automake' to read the Automake manual.
EOF


cat <<'EOF' >> Makefile.am
SUBDIRS = src
dist_doc_DATA = README
EOF

cat <<'EOF' >> configure.ac
AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
EOF

autoreconf --install 

./configure 

make 

make distcheck
