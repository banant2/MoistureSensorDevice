--init.lua
--enable measurement
gpio.mode(5,gpio.OUTPUT)
gpio.write(5,gpio.HIGH)
wifi.setmode(wifi.station)
wifi.sta.config("BA Network", "3146308066")
wifi.sta.connect()
tmr.alarm(1, 5000,tmr.ALARM_AUTO, function()


    tmr.alarm(2,4000,1, function()

        if wifi.sta.getip() == nil then
            --keep trying--
            print ("Waiting for IP")
        else
            -- nice, we're connected
            tmr.stop(2)
            print ("IP is:" ..wifi.sta.getip())

            --measure ADC and get output
            moisture=adc.read(0)
            gpio.mode(5, gpio.LOW)
            print("Moisture = "..moisture .."\n")

            print("Sending data to thingspeak.com")
            --setup connection

            print("Sending data to thingspeak.com")
            conn=net.createConnection(net.TCP,0)
            conn:on("receive", function(conn, payload)
                if (string.find(payload, "Status: 200 OK") ~= nil) then
                    print("Posted OK")
                end
            end)

            conn:on("connection", function(connout, payload)
 
                print ("Posting...");
                conn:send("GET /update?key=UI2614YOMH9BVM73&field1="..moisture
                        .. " HTTP/1.1\r\n"
                        .. "Host: api.thingspeak.com\r\n"
                        .. "Connection: close\r\n"
                        .. "Accept: */*\r\n"
                        .. "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n"
                        .. "\r\n")
                tmr.alarm(3, 1000, 1, function()
                    tmr.stop(3)
                    print("timeout")
                    node.dsleep(30000000)
                    conn:close()
                end)
            end)
                
            conn:on("sent",function(conn,payload)
                print("Closing connection")
                node.dsleep(30000000)
                conn:close()
                collectgarbage()
            end)
            conn:connect(80,'api.thingspeak.com')
        end
    end)
    print("got to end of code")
end)
        



