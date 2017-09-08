This will be an example that shows connecting to Exosite's Murano.

It is on the [AT88CKECC-AWS-XSTK](http://www.atmel.com/tools/at88ckecc-aws-xstk.aspx)
dev kit from [Atmel](http://www.atmel.com),
utilizing the [Exosite C library](https://github.com/exosite/libexosite_c_http-device).


1. Install Atmel Studio
2. Install git
3. `git clone https://github.com/exosite/example_c_atsamg55XplainedPRO_atmelStudio.git`
4. `cd example_c_atsamg55XplainedPRO_atmelStudio`
5. `git submodule init`
6. `git submodule update`
7. In Atmel Studio: Open > Project/Solution…
8. Navigate to `example_c_atsamg55XplainedPRO_atmelStudio.atsln`
9. Open the file `example_c_atsamg55XplainedPRO_atmelStudio/src/config/conf_exosite.h`.
10. Edit the `#define MAIN_WLAN_SSID`, replacing `<SSID>` with the SSID of your secure Wi-Fi access point (e.g., linksys, NETGEAR, etc.).
11. Edit the `#define MAIN_WLAN_PSK`, replacing `<PASSWORD>` with the password to your Wi-Fi access point.
12. Edit the `#define EXOPAL_PRODUCT_ID`, replacing `<PRODUCT_ID>` with your Product ID from the first step in this tutorial.
13. Save the file.
14. Build > Rebuild Solution
15. Debug > Start Without Debugging (Ctrl+Alt+F5)

