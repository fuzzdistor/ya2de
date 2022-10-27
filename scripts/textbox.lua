local my = {}

local function textFeeder()
    local i = 1
    while i <= #my.text do
        if node:getTextWidth() > node.box_width then
            local last_space = 1
            while my.text:sub(i-last_space,i-last_space) ~= " " do
                last_space = last_space + 1
            end
            my.text = my.text:sub(1,i-last_space-1)..'\n'..my.text:sub(i-last_space+1)
        end
        node:setString(my.text:sub(1,i))

        node.sound:stop()
        if my.text:sub(i,i):match("%w") then
            node.sound:setPitch(my.pitch - 0.1 + math.random()/10)
            node.sound:play()
        end
        coroutine.yield()
        i = i+1
    end
end

function init()
    node:setScale(1, 1)
    node:setPosition(-300, 160)
    node:setCharacterSize(24)
    node:setOutlineThickness(3)

    math.randomseed(19)
    my.text = node:getDialogueLine(0)

    my.acc_time = 0
    my.deb_acc_time = 0
    my.text_speed = 0.05

    my.pitch = 1

    feedText = coroutine.create(textFeeder)
end

function handler(message)
    return "error! " .. message
end

function update(dt)
    if checkAction("action_b") then my.text_speed = 0.01 end
    if checkAction("action_a") and not my.debounceflag then
        if node:nextLine() then
            my.text = node:getDialogueLine(node.current_line_index)
            feedText = coroutine.create(textFeeder)
            my.text_speed = 0.05
        end
        my.debounceflag = true;
    end
    if my.debounceflag then
        my.deb_acc_time = my.deb_acc_time + dt;
        if my.deb_acc_time > 1 then
            my.deb_acc_time = 0
            my.debounceflag = false
        end
    end
    my.acc_time = my.acc_time + dt
    if my.acc_time > my.text_speed then
        my.acc_time = my.acc_time - my.text_speed
        coroutine.resume(feedText)
    end
end
