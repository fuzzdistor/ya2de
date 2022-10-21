this:setScale(1, 1)
this:setPosition(-300, 160)
text:setCharacterSize(36)
text:setOutlineThickness(4)
setString("")

math.randomseed(19)

local my = {}

my.acc_time = 0
my.text_speed = 0.05

my.text = "This is a demo string that fits inside the textbox!"
my.pitch = 1
my.size = getTextWidth()

local feedText = coroutine.create(function ()
    local i = 1
    local last_space = 1
    while i <= #my.text do
        if my.size > 600 then
            -- TODO find last space to insert the line jump
            last_space = 1
            while my.text:sub(i-last_space,i-last_space) ~= " " do
                last_space = last_space + 1
            end
            my.text = my.text:sub(1,i-last_space)..'\n'..my.text:sub(i-last_space+1)
        end
        setString(my.text:sub(1,i))
        sound:stop()
        if my.text:sub(i,i):match("%w") then
            sound:setPitch(my.pitch - 0.1 + math.random()/10)
            sound:play()
        end
        coroutine.yield()
        i = i+1
    end
end)

function handler(message)
    return "error! " .. message
end

function update(dt)
    if checkAction("move_up") then my.text_speed = 0.01 end
    my.size = getTextWidth()
    my.acc_time = my.acc_time + dt
    if my.acc_time > my.text_speed then
        my.acc_time = my.acc_time - my.text_speed
        coroutine.resume(feedText)
    end
end
