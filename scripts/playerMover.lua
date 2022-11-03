local my = {}

function takeDamage()
    my.life = my.life - 1
    print("took damage!")
    local color = node.sprite:getColor()
    local newcolor = Color.new(color.r, color.g * 0.8, color.b * 0.8, color.a)
    node.sprite:setColor(newcolor)
    if my.life < 1 then
        node:markForDestruction()
        print("you died!")
    end
end

function init()
    my.speed = 100;
    my.triggerflag = false;
    my.life = 10;

    node:setAnimationInfo(3, 3, 1)
    node:setFrame(0)
    node:setOrigin(16, 32)
    my.animation_frame = 1
end

local animationUpdater = function(dt)
    if not my.anim then
        my.anim = {}
        my.anim.timer = 0
    end

    my.anim.timer = my.anim.timer + dt
    if my.anim.timer > 0.1 then
        my.anim.timer = my.anim.timer - 0.1
        my.animation_frame = my.animation_frame + 1
        if my.animation_frame > node:getFrameCount() then
            my.animation_frame = 1
        end
        node:setFrame(my.animation_frame)
    end
end

function update(dt)
    if(triggercallback == true) then
        node:setRotation(node:getRotation() + 360 * dt)
    end
    direction = Vector2.new(0,0)
    if checkAction("move_up")       then direction.y = -1 end
    if checkAction("move_down")     then direction.y = 1 end
    if checkAction("move_left")     then direction.x = -1 end
    if checkAction("move_right")    then direction.x = 1 end

    animationUpdater(dt)

    direction:normalize()
    node:move(direction.x * dt * my.speed, direction.y * dt * my.speed)
end

