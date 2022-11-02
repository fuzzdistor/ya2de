local my = {}
my.flag = false

node.callback = function(other_node, dt)
    local child_count = 10000
    if not my.flag then
        for _=1, child_count do
            other_node:attachChild(SceneNode.new())
        end
        print("Done! Attached "..child_count.." nodes to the player!")
        my.flag = true
    end
end

my.timer = 0

node.callback = function(other_node, dt)
    my.timer = my.timer + dt
    if my.timer > 0.2 then
        my.timer = my.timer - 0.2
        other_node:invoke("takeDamage")
    end
end

node:setPosition(30, 30)
