local my = {}

local feedText = coroutine.create(function ()
    local i = 1
    while i <= #my.text do
        if text:getTextWidth() > 600 then
            local last_space = 1
            while my.text:sub(i-last_space,i-last_space) ~= " " do
                last_space = last_space + 1
            end
            my.text = my.text:sub(1,i-last_space-1)..'\n'..my.text:sub(i-last_space+1)
        end
        text:setString(my.text:sub(1,i))
        sound:stop()
        if my.text:sub(i,i):match("%w") then
            sound:setPitch(my.pitch - 0.1 + math.random()/10)
            sound:play()
        end
        coroutine.yield()
        i = i+1
    end
end)

function init()
    this:setScale(1, 1)
    this:setPosition(-300, 160)
    text:setCharacterSize(24)
    text:setOutlineThickness(3)

    math.randomseed(19)
    my.text = text:getString()
    text:setString("");

    my.acc_time = 0
    my.text_speed = 0.05

    my.pitch = 1
end

function handler(message)
    return "error! " .. message
end

function update(dt)
    if checkAction("move_up") then my.text_speed = 0.01 end
    my.acc_time = my.acc_time + dt
    if my.acc_time > my.text_speed then
        my.acc_time = my.acc_time - my.text_speed
        coroutine.resume(feedText)
    end
end
