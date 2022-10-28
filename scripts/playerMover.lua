local my = {}

my.speed = 100;
my.triggerflag = false;

function update(dt)
    if(triggercallback == true) then
        node:setRotation(node:getRotation() + 360 * dt)
    end
    direction = Vector2.new(0,0)
    if checkAction("move_up")       then direction.y = -1 end
    if checkAction("move_down")     then direction.y = 1 end
    if checkAction("move_left")     then direction.x = -1 end
    if checkAction("move_right")    then direction.x = 1 end
    direction:normalize()
    node:move(direction.x * dt * my.speed, direction.y * dt * my.speed)
end

