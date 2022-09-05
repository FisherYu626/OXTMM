# OXTMM

You may want to test different following total number of supporting file and deletion proportion in CryptoTestingApp/CryptoTestingApp.cpp.

## How to Build/Execute the Application 
1. Install Intel(R) SGX SDK for Linux* OS
2. Build the project with the prepared Makefile:

   Using Hardware Mode and Debug build:
   
       `` $ cd SE_SGX_1 && make clean``
       
       `` $ make SGX_MODE=HW SGX_DEBUG=1``

3. Execute the binary directly:
  `
    $ ./cryptoTestingApp
  `

### sgx_create_enclave failed: 0x4001

1. Check the SGX driver:

$ ls /dev | grep sgx



2. Check the PSW, especially libsgx-uae-service

$ sudo apt list --installed | grep sgx

 

3. Run sudo apt-get update to ensure all your components are up-to-date.

 

4. Setup your proxy if necessary.

Edit the file 
/etc/aesmd.conf
 (refer to the comments in the file) to set the proxy for the aesmd service.
After you configure the proxy, you need to restart the service to enable the proxy.

 

5. Check aesmd

$ ps aux | grep -aesm

$ sudo systemctl stop aesmd

$ sudo systemctl start aesmd

$ cat /var/log/syslog | grep -i aesm