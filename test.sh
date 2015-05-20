#########################################################################
# File Name: test.sh
# Author: lunan
# mail: 6616@shuncom.com
# Created Time: 2015年05月14日 星期四 13时06分34秒
#########################################################################
#!/bin/zsh
cd ~/igateway/
make package/network/services/linux/serial2tcp/compile V=s
scp build_dir/target-mips_34kc_uClibc-0.9.33.2/serial2tcp/serial2tcp root@192.168.99.228:/tmp/
cd -
