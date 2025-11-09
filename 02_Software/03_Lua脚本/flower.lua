-------------------------------------------------------------------------------
-- 注册C函数                                                                 --
-- u2f 将32位整型内存数据转换为浮点数（不同于值转换）                        --
-- 类似C代码 *(float*)(&u)                                                   --
-- function u2f(u)                                                           --
-- @param   u   number   整数值                                              --
-- @return  成功返回浮点数值，否则返回nil                                    --
-- @example local u = 123                                                    --
--          local f = u2f( 123 )                                             --
--                                                                           --
-- time 获取时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒数            --
-- function time()                                                           --
-- @return  返回当前时间戳                                                   --
-- @example local t = time()                                                 --
--                                                                           --
-- year 获取年（year-1900）                                                  --
-- function year(t)                                                          --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回年                                                           --
-- @example local y = year( t )                                              --
--                                                                           --
-- month 获取月（0-11）                                                      --
-- function month(t)                                                         --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回月                                                           --
-- @example local m = month( t )                                             --
--                                                                           --
-- day 获取日（1-31）                                                        --
-- function day(t)                                                           --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回月                                                           --
-- @example local d = day( t )                                               --
--                                                                           --
-- hour 获取时（0-23）                                                       --
-- function hour(t)                                                          --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回时                                                           --
-- @example local h = hour( t )                                              --
--                                                                           --
-- minute 获取分（0-59）                                                     --
-- function minute(t)                                                        --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回分                                                           --
-- @example local m = minute( t )                                            --
--                                                                           --
-- second 获取秒（0-59）                                                     --
-- function second(t)                                                        --
-- @param   t   number   时间戳，距离（00:00:00 UTC, January 1, 1970）的秒数 --
-- @return  返回秒                                                           --
-- @example local m = second( t )                                            --
-------------------------------------------------------------------------------

--------------------------------------------------------
-- 将bytes string转换hex string                       --
-- @param   s   string   bytes string                 --
-- @return  返回hex string，类似"0A0B0C0D..."         --
-- @example local hex = to_hex("\2\2\0\150\0\37\206") --
--------------------------------------------------------
function to_hex(s)
        local i
        local t

        t={s:byte(1,s:len())}
        for i=1,#t do
                t[i]=string.format('%02X',t[i])
        end

        return table.concat(t)
end

-----------------------------------------------
-- 将object序列化成字符串                    --
-- @param   o   boolean|number|string|table  --
-- @return  返回序列化string                 --
-- @example local str = to_str({x=100})      --
-----------------------------------------------
function to_str(o)
        local i=1
        local t={}
        local f

        f=function(x)
                local y=type(x)
                if y=="number" then
                        t[i]=x
                        i=i+1
                elseif y=="boolean" then
                        t[i]=tostring(x)
                        i=i+1
                elseif y=="string" then
                        t[i]="\""
                        t[i+1]=x
                        t[i+2]="\""
                        i=i+3
                elseif y=="table" then
                        t[i]="{"
                        i=i+1

                        local z=true
                        for k,v in pairs(x) do
                                if z then
                                        z=false
                                        t[i]="\""
                                        t[i+1]=k
                                        t[i+2]="\""
                                        t[i+3]=":"
                                        i=i+4
                                        f(v)
                                else
                                        t[i]=","
                                        t[i+1]="\""
                                        t[i+2]=k
                                        t[i+3]="\""
                                        t[i+4]=":"
                                        i=i+5
                                        f(v)
                                end
                        end

                        t[i]="}"
                        i=i+1
                else
                        t[i]="nil"
                        i=i+1
                end
        end
        f(o)

        return table.concat(t)
end

----------------------------------------------------------------------------------------------------------
-- 添加值数据点到table中                                                                                --
-- @param   t   table                                                                                   --
--          i   string                      数据流或数据流模板名称                                      --
--          a   number                      毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒； --
--                                          如果值为0，表示使用当前时间                                 --
--          v   boolean|number|string|table 布尔值、数值、字符串、json                                  --
--          c   string                      用于标识数据点归属(设备AuthCode,可选)                       --
--                                          如果值为“”或nil，表示数据点归属建立TCP连接的设备            --
-- @return  成功返回true，否则返回false                                                                 --
-- @example local ok = add_val(t,"dsname",0,100)                                                        --
----------------------------------------------------------------------------------------------------------
function add_val(t, i, a, v, c)
        if type(t)~="table" then
                return false
        elseif type(i)~="string" then
                return false
        elseif type(a)~="number" then
                return false
        else
                local o = type(v)
                if o~="boolean" and o~="number" and o~="string" and o~="table" then
                        return false
                end

                local n = {i=i,v=v}
--                n.v = n.v .. "("..o..")"
                if a~=0 and a~=nil then
                        n["a"]=a
                end
                if c~=nil then
                        n["c"]=c
                end

                -- list push_back --
                if t.h==nil then
                        t.h={nil,n}
                        t.t=t.h
                else
                        t.t[1]={nil,n}
                        t.t=t.t[1]
                end
        end

        return true
end
----------------------------------------------------------------------------------------------------------
-- 添加二进制数据点到table中                                                                            --
-- @param   t   table                                                                                   --
--          i   string                      数据流或数据流模板名称                                      --
--          a   number                      毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒； --
--                                          如果值为0，表示使用当前时间                                 --
--          b   string                      二进制数据（hex string），类似"0A0B0C0D..."                 --
--          d   boolean|number|string|table 用于描述b（可选）；数值、字符串、json                       --
--          c   string                      用于标识数据点归属(设备AuthCode,可选)                       --
--                                          如果值为“”或nil，表示数据点归属建立TCP连接的设备            --
-- @return  成功返回true，否则返回false                                                                 --
-- @example local ok = add_val(t,"dsname",0,"0A0B0C0D...",{...})                                        --
----------------------------------------------------------------------------------------------------------
function add_bin(t, i, a, b, d, c)
	if type(t)~="table" then
		return false
	elseif type(i)~="string" then
		return false
	elseif type(a)~="number" then
		return false
	elseif type(b)~="string" then
		return false
	else
		local o=type(d)
		if o~="nil" and o~="string" and o~="table" then
			return false
		end

		local n={i=i,b=to_hex(b)}
		if a~=0 and a~=nil then
			n["a"]=a
		end
		if d~=nil then
			n["d"]=d
		end
		if c~=nil then
			n["c"]=c
		end

		-- list push_back --
		if t.h==nil then
			t.h={nil,n}
			t.t=t.h
		else
			t.t[1]={nil,n}
			t.t=t.t[1]
		end
	end

	return true
end

--------------------------------------------------------
-- 打印table内容                                      --
-- @param   t   table                                 --
-- @return  无                                        --
-- @example local ok = add_val({},"dsname",0,100)     --
--          print_table(t)                            --
--------------------------------------------------------
function print_table(t)
        print("data list ->")
        local i,n
        n=t.h
        while n~=nil do
                for i,v in pairs(n[2]) do
                        print(i,v)
                end
                n=n[1]
        end
end

--------------------------------------------------------
-- 打印table数据，转换成JSON                          --
-- @param   t   table                                 --
-- @return  返回JSON字符串                            --
-- @example local ok = add_val({},"dsname",0,100)     --
--          local json = table_to_json(t)             --
--------------------------------------------------------
function table_to_json(t)
        local s="{\"datastreams\":["
        local i,n
        n=t.h
        while n~=nil do
                s=s.."{\"id\":\""..n[2].i.."\",\"datapoints\":["
                s=s.."{\"at\":\""
                if n[2].a==nil then
                        s=s..time()
                else
                        s=s..n[2].a
                end
                s=s.."\",\"value\":"
                local y=type(n[2].v)
                if y=="string" then
                        s=s.."\""..n[2].v.."\""
                elseif y=="boolean" or y=="number" then
                        s=s..tostring(n[2].v)
                elseif y=="table" then
                        s=s..to_str(n[2].v)
                else
                        s=s.."nil"
                end
                s=s.."}]},"
                n=n[1]
        end
        s=s.."]}"
        return s
end

-- 解析设备上传数据
-- @param  dev    user_data   设备管理器
-- @return size表示已解析设备上传数据的字节数，json表示解析后的数据点集合

function device_data_analyze(dev)
        local t = {}
        local a = 0
        
        -- 获取上行数据长度
        local s = dev:size()
        local data = dev:bytes(1, s)
        
        -- 初始化传感器数据的位置
        local temp, humi, lux, mq5_ppm, humi_soil, ph = 0, 0, 0, 0, 0, 0
        local temp_end, humi_end, lux_end, mq5_ppm_end, humi_soil_end, ph_end, str_end = 0, 0, 0, 0, 0, 0, 0
        
        -- 解析传感器数据
        -- temp, temp_end = string.find(data, "T(%d+%.?%d*)")
        -- humi, humi_end = string.find(data, "H(%d+%.?%d*)")
        -- lux, lux_end = string.find(data, "L(%d+%.?%d*)")
        -- mq5_ppm, mq5_ppm_end = string.find(data, "M(%d+%.?%d*)")
        -- humi_soil, humi_soil_end = string.find(data, "S(%d+%.?%d*)")
        -- ph, ph_end = string.find(data, "P(%d+%.?%d*)")
        
        -- if temp then
        --     add_val(t, "temp", a, tonumber(data:sub(temp + 1, temp_end)))
        -- end
        -- if humi then
        --     add_val(t, "humi", a, tonumber(data:sub(humi + 1, humi_end)))
        -- end
        -- if lux then
        --     add_val(t, "Lux", a, tonumber(data:sub(lux + 1, lux_end)))
        -- end
        -- if mq5_ppm then
        --     add_val(t, "combustible_gas", a, tonumber(data:sub(mq5_ppm + 1, mq5_ppm_end)))
        -- end
        -- if humi_soil then
        --     add_val(t, "humi_soil", a, tonumber(data:sub(humi_soil + 1, humi_soil_end)))
        -- end
        -- if ph then
        --     add_val(t, "ph", a, tonumber(data:sub(ph + 1, ph_end)))
        -- end

        temp, str_end = string.find(dev:bytes(1, s),"T")
        humi, str_end = string.find(dev:bytes(1, s),"H")
        lux, str_end = string.find(dev:bytes(1, s),"L")
        mq5_ppm, str_end = string.find(dev:bytes(1, s),"M")
        humi_soil, str_end = string.find(dev:bytes(1, s),"S")
        ph, str_end = string.find(dev:bytes(1, s),"P")

        add_val(t,"temp",a,dev:bytes(temp+1, humi-temp-1)) --添加到datastream数据流
        add_val(t,"humi",a,dev:bytes(humi+1, lux-humi-1))
        add_val(t,"Lux",a,dev:bytes(lux+1, mq5_ppm-lux-1))
        add_val(t,"combustible_gas",a,dev:bytes(mq5_ppm+1, humi_soil-mq5_ppm-1))
        add_val(t,"humi_soil",a,dev:bytes(humi_soil+1, ph-humi_soil-1))
        add_val(t,"ph",a,dev:bytes(ph+1, s-ph))
    
        -- 解析按键信息
        local cmd = data:sub(1, 1)
        local button_data = data:sub(2)
    
        if cmd == "S" then
            local switch_data = button_data:sub(1, 1)
            local state = button_data:sub(2)
            if switch_data == "P" then
                if state == "1" then
                    print("Water Pump ON")
                    -- 控制水泵开关逻辑
                else
                    print("Water Pump OFF")
                    -- 控制水泵关闭逻辑
                end
            elseif switch_data == "F" then
                if state == "1" then
                    print("Fan ON")
                    -- 控制风扇开关逻辑
                else
                    print("Fan OFF")
                    -- 控制风扇关闭逻辑
                end
            elseif switch_data == "L" then
                if state == "1" then
                    print("Light Panel ON")
                    -- 控制灯板开关逻辑
                else
                    print("Light Panel OFF")
                    -- 控制灯板关闭逻辑
                end
            end
        else
            print("Unknown command")
        end
    
        dev:response()
        dev:send("received")  --发送应答
    
        return s, to_json(t)  --保存该数据
    end
    