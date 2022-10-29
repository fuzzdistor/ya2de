# YA2DE Lua API reference

This is going to be ~hell~ fun.

## Index

### Nodes

- [SceneNode](#scenenode)
- [SpriteNode](#spritenode)
- [TileMapNode](#tilemapnode)
- [ShapeNode](#shapenode)
- [YSortNode](#ysortnode)
- [TextNode](#textnode) UNDER CONSTRUCTION
- [TextboxNode](#textboxnode)
- [SoundPlayerNode](#soundplayernode)
- [TriggerNode](#triggernode)
- [AreaSwitchNode](#areaswitchnode)

### Common Types

- [Vector2](#vector2) UNDER CONSTRUCTION
- [Color](#color) UNDER CONSTRUCTION
- [Sound](#sound) UNDER CONSTRUCTION

## Basic Overview

YA2DE sets bindings to Nodes methods and members as they are needed and
leaves it available for you through the `node` object inside of Lua.

The engine also sets up some basic utility objects like `sf::Vector2f` and
a basic binding to `sf::Color` for every node. Special bindings for objects
like `sf::Sound` are available for nodes like `SoundPlayerNode` that use them.

You can access all the available bindings for a Node object through the
`node` variable in your Lua scritps like this:

```lua
-- using it in a TriggerNode

-- calling the C++ setPosition function that admits 2 floats as parameters
node:setPosition(200, 400)

-- creating a C++ sf::Vector2f inside lua
local myNewPosition = Vector2.new(100, 100)

-- setting the function callback for the trigger node
node.callback = function(othernode, dt)
    othernode:setPosition(myNewPosition)
end
```

---

## Text

Texts are the central elements for TextNodes and TextboxNodes. You can
manipulate them to suit your needs.

### :setCharacterSize

```lua
node:setCharacterSize(size)     -- sf::Text::setCharacterSize(unsigned int size)
```

- Arguments
    - **number** **`size`**: size of the font. It will be implicitly casted to
    an unsigned int
- Returns
    - Nothing

### :setFillColor

```lua
node:setFillColor(color)     -- sf::Text::setFillColor(const sf::Color& color)
```

- Arguments
    - [**Color**](#Color) **`color`**: color for the inside of the text
- Returns
    - Nothing

### :setOutlineColor

```lua
node:setOutlineColor(color)     -- sf::Text::setOutlineColor(const sf::Color& color)
```

- Arguments
    - [**Color**](#Color) **`color`**: color for the outline of the font
- Returns
    - Nothing

### :setOutlineThickness

```lua
node:setOutlineThickness(thickness)     -- sf::Text::setOutlineThickness(float thickness)
```

- Arguments
    - **number** **`thickness`**: the thickness of the outline for the text
- Returns
    - Nothing

### :setString

```lua
node:setString(string)    -- [](sf::Text& text, const std::string& string) { text.setString(string); }
```

- Arguments
    - **string** **`string`**: string for the text
- Returns
    - Nothing

### :getString

```lua
string = node:getString()    -- [](const sf::Text& text){ return std::string(text.getText()); }
```

- Arguments
    - **string** **`string`**: string for the text
- Returns
    - Nothing

### :getBounds

```lua
rect = node:getBounds()     -- sf::TextNode::getLocalBounds() -> sf::FloatRect
```

- Arguments
    - None
- Returns
    - **FloatRect** **`rect`**: Axis aligned bounding box. There is no way of
    manipulating a FloatRect inside of Lua yet.

> TODO: Add FloatRect bindings

---

## SceneNode

SceneNode is the base node from which all nodes that compose a scene are made
of (directly or indirectly). It provides the necessary elements for a node to
be transformed (moved, scaled and rotated), be drawn to the screen, be marked
for destruction and much more.

### :move

```lua
node:move(x, y)     -- SceneNode::move(float x, float y)
```

- Arguments
    - **number `x`**: offset to add to current X coordinate
    - **number `y`**: offset to add to current Y coordinate
- Returns
    - Nothing
- Notes
    - `move` is different from [`setPosition`](#setposition) in that move will move the node
      relative to the current position

### :setPosition

```lua
node:setPosition(x, y)     (1) -- SceneNode::setPosition(float x, float y)
node:setPosition(position) (2) -- SceneNode::setPosition(const Vector2f& position)
```

- Arguments (1)
    - **number** **`x`**: X coordinate of the new position
    - **number** **`y`**: Y coordinate of the new position
- Arguments (2)
    - [**Vector2**](#vector2) **`position`**: coordinates of the new position
- Returns
    - Nothing

### :setScale

```lua
node:setScale(x, y) -- SceneNode::setScale(float x, float y)
```

- Arguments
    - **number** **`x`**: new hoizontal scale factor
    - **number** **`y`**: new vertical scale factor
- Returns
    - Nothing

### :rotate

```lua
node:rotate(degrees) -- SceneNode::rotate(float degrees)
```

- Arguments
    - **number** **`degrees`**: angle in degrees to be added to current rotation
- Returns
    - Nothing
- Notes
    - `rotate` is different from `setRotation` in that it adds relative to the
      current rotation instead of specifiying a new one

### :setRotation

```lua
node:setRotation(degrees) -- SceneNode::setRotation(float degrees)
```

- Arguments
    - **number** **`degrees`**: new absolute rotation angle in degrees
- Returns
    - Nothing

### :getRotation

```lua
angle = node:getRotation() -- SceneNode::getRotation() -> float
```

- Arguments
    - None
- Returns
    - **number** **`angle`**: absolute angles in degrees

### :getScale

```lua
scale = node:getScale() -- SceneNode::getScale -> Vector2f
```

- Arguments
    - None
- Returns
    - [**Vector2**](#vector2) **`scale`**: scale factor of the node

### :getPosition

```lua
position = node:getPosition() -- SceneNode::getPosition() -> Vector2f
```

- Arguments
    - None
- Returns
    - [**Vector2**](#vector2)**`position`**: coordinates of the node's position

---

## SpriteNode

SpriteNodes can display a texture on the scene. They have methods for setting a
sprite's origin point.

> TODO: come back when there is more functionality added

---

## TileMapNode

TileMapNodes can hold and display square or isometric layered tilemaps. Their
logic is tied to TileSets which are a resource type that holds the texture,
texture coordinates and ids for each tile.

It has an special usertype allied with it: The Cell type

> TODO: add some cell functionality and come back

> TODO: come back when there is more functionality added

---

## ShapeNode

ShapeNodes are nodes that have a setteable area that can be used as a
collision area. They can also be used for displaying a primitive shape on the
screen.

### .enabled

```lua
node.enabled    -- bool ShapeNode::m_enabled
```

- **boolean** Propierty: it determines if the bounding box is active or not
- Notes
    - Do not get `enabled` confused with [`visible`](#visible). A ShapeNode
    can be disabled and visible and vice versa

### .visible

```lua
node.visible    -- bool ShapeNode::m_visible
```

- **boolean** Propierty: it determines if the shape should be drawn to the
  scene.
- Notes
    - Do not get `visible` confused with [`enabled`](#enabled). A ShapeNode
      can be invisible and enabled and vice versa

### :setFillColor

```lua
node:setFillColor(color) -- ShapeNode::setFillColor(const sf::Color color)
```

- Arguments
    - [**Color**](#color) **`color`**: color for the inside of the shape
- Returns
    - Nothing

### :setSize

```lua
node:setSize(x, y) (1) -- ShapeNode::setSize(float w, float h)
node:setSize(size) (2) -- ShapeNode::setSize(const sf::Vector2f size)
```

- Arguments
    - **number x**: horizontal size for the shape
    - **number y**: vertical size for the shape
- Returns
    - Nothing

### :getFillColor

```lua
color = node:getFillColor() -- ShapeNode::getFillColor() -> sf::Color
```

- Arguments
    - None
- Returns
    - [**Color**](#color) **`color`**: color of the inside of the shape

---

## YSortNode

> TODO: YSortNode blurb


---

## TextNode

> TODO: TextNode blurb

> TODO: Special type Sound

### .sound

```lua
node.sound      -- sf::Sound TextNode::m_sound
```

- [**Sound**](#sound) Propierty: the soundbuffer responsible for the "Animal
  Crossing"/"Undertale" style chirping sound. It will be pushed to its own
  SoundPlayerNode in the future.

### .text

```lua
node.text     -- TextNode::m_text
```

- [**Text**](#text) Propierty: the text object that gets drawn into the screen.
You can learn more about how to manipulate Text objects [here](#text)

### :getTextWidth

```lua
width = node:getTextWidth()     -- TextNode::getTextWidth() -> float
```

- Arguments
    - None
- Returns
    - **number** **`width`**: The current width of the text texture. This takes
    into account the font used, that means that the same text will return a
    greater width when using a wide font compared to using a thin font

---

## TextboxNode

> TODO: TextboxNode blurb

### .dialogue

```lua
node.dialogue       -- std::vector<std::string> TextboxNode::m_dialogueLines
```

- **table (array)** Propierty: This table contains all the dialogue strings. It
is ipairs iterable.

### .box_width

```lua
node.box_width      -- TextboxNode::m_textboxWidth
```

- **number** Propierty: is the width in pixels for the textbox. It's used for
wrapping text inside the box. In the future it will be a hidden propierty

### :nextLine

```lua
did_advance = node:nextLine()      -- TextboxNode::nextLine() -> boolean
```

- Arguments
    - None
- Returns
    - **boolean** **`did_advance`**: it is `true` if the function triggered a line
    advance and `false` if it didn't
- Notes
    - It triggers a line advance. If the index is in the last dialogue it does
    not advance

### :getCurrentLineIndex

```lua
line = node:getCurrentLineIndex()     -- size_t TextboxNode::m_currentLineIndex
```

- Arguments
    - None
- Returns
    - **number** **`line`**: The current dialogue line index

### :getDialogueLine

```lua
line = node:getDialogueLine(index)  -- TextboxNode::getDialogueLine(size_t index) -> std::string
```

- Arguments
    - **number** **`index`**: index is 1-based. Line index to be requested. If
    the index is out of bounds it returns the last line.
- Returns
    - **string** **`line`**: The string corresponding to the 0-based
    transformed index.

---

## SoundPlayerNode

SoundPlayerNodes take care of playing sounds or music. They are smart in the
sense that they will choose between using an in-memory object or a stream object
depending on the sound source you provide for them.

> TODO: make a more robust api

### :play

```lua
usernode:play()     -- SoundPlayerNode::play
```

- Arguments
    - None
- Returns
    - Nothing
- Notes
    - It will play the sound or music if it was stopped, it will resume it if
    it was paused, and will restart it from the beginning if it was already
    playing.

### :stop

```lua
node:stop()     -- SoundPlayerNode::stop
```

- Arguments
    - None
- Returns
    - Nothing
- Notes
    - It will stop the sound if it is playing or paused and do nothing if it is
    already stopped.

---

## TriggerNode

Triggernode blurb


### :callback

```lua
node:callback     --std::function<void(SceneNode*, float)> TriggerNode::m_callback
```

- **function** Propierty: this function will be called when a Player mask node
collides with the bounding box of a child ShapeNode

---

## AreaSwitchNode

AreaSwitchNodes are kind of like [TriggerNodes](#triggernode) but are specialized
for switching to other scenes

### .destiny_area

```lua
node.destiny_area       -- std::string AreaSwitchNode::m_destinyArea
```

- **string** Propierty: It indicates the scene name for the scene switch
