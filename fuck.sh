#!/bin/bash
#
# Force Apt to use ipv4
#


# https://gist.github.com/joshgachnang/2306795
if [ "$(whoami)" != "root" ]; then
        echo "Script must be run as: root|sudo"
        exit 2
fi

# https://jinnabalu.medium.com/create-a-file-in-bash-using-eof-af5af33c0d1f
# https://www.vultr.com/docs/force-apt-get-to-ipv4-or-ipv6-on-ubuntu-or-debian
cat << 'EOF' > /etc/apt/apt.conf.d/99force-ipv4
Acquire::ForceIPv4 "true";
EOF
