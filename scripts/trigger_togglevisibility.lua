local latcher = {
    _flag = false,
    _latch = false,
    update = function(self)
        if not self._flag then
            self._latch = false
        end
        self._flag = false
    end,
    lock = function(self)
        self._latch = true
    end,
    flag = function(self)
        self._flag = true
    end,
    locked = function(self)
        return self._latch
    end
}

function init()
    node.callback = function(othernode, dt)
        if not latcher:locked() then
            othernode.visible = not othernode.visible
            latcher:lock()
        end
        latcher:flag()
    end
end

function update(dt)
    latcher:update()
end
