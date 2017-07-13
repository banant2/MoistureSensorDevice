gpio.mode(5,gpio.OUTPUT)
gpio.write(5,gpio.HIGH)
i=0
if i<50 then
    tmr.alarm(1, 5000,tmr.ALARM_AUTO, function()
        moisture=adc.read(0)
        print("Moisture = "..moisture .."\n")
        i=i+1
    end)
else
    print("all done")
end
