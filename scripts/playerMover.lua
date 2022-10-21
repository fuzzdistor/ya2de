this:setPosition(100,20)

local my = {}

my.speed = 100;

function update(dt)
    direction = Vector2f.new(0,0)
    if checkAction("move_up")       then direction.y = -1 end
    if checkAction("move_down")     then direction.y = 1 end
    if checkAction("move_left")     then direction.x = -1 end
    if checkAction("move_right")    then direction.x = 1 end
    direction:normalize()
    this:move(direction.x * dt * my.speed, direction.y * dt * my.speed)
end

