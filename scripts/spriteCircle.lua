local flux = require("scripts.lib.flux")

local my = {}

print(node:getPosition())
my.position = node:getPosition()
my.scale = Vector2:new(5,5)

local circleCoordsX = {
    node:getPosition().x - 200 * node:getScale().x,
    node:getPosition().x,
}

local circleCoordsY = {
    node:getPosition().y + 100 * node:getScale().y,
    node:getPosition().y - 100 * node:getScale().y,
    node:getPosition().y,
}

local function circleMotion()
    flux.to(my.position, 1, {y=circleCoordsY[1]})
        :ease("sineout")
        :after(2, {y=circleCoordsY[2]})
        :ease("sineinout")
        :after(1, {y=circleCoordsY[3]})
        :ease("sinein")
    flux.to(my.position, 2, {x=circleCoordsX[1]})
        :ease("sineinout")
        :after(2, {x=circleCoordsX[2]})
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
    node:setPosition(my.position)
end

