local flux = require("scripts.lib.flux")

local my = {}

my.position = Vector2f:new(100,0)
my.scale = Vector2f:new(5,5)

local function circleMotion()
    flux.to(my.position, 1, {y=100})
        :ease("sineout")
        :after(2, {y=-100})
        :ease("sineinout")
        :after(1, {y=0})
        :ease("sinein")
    flux.to(my.position, 2, {x=-100})
        :ease("sineinout")
        :after(2, {x=100})
        :ease("sineinout")
        :oncomplete(circleMotion)
end

local function tweens()
    flux.to(my.position, 2, {x = 100, y = 200})
        :ease("elasticout")
        :after(2, {x = 0, y = 0})
    flux.to(my.scale, 2, {x = 2, y = 2})
        :ease("elasticout")
        :after(2, {x = 1, y = 1})
        :oncomplete(tweens)
end

circleMotion()

my.speed = 100;

-- known issue: if the window is being resized the circle motion
-- will not keep calling itself until the window is left alone again
function update(dt)
    flux.update(dt)
    this:setPosition(my.position)
end

