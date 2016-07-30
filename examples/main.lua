local image = love.graphics.newImage("hey.png")
image:setFilter("nearest","nearest")

function love.load()
	love.window.setTitle("simple example 1")
	love.window.setMode(600,600)
end

function love.draw()
	love.graphics.setBackgroundColor(100,55,60,255)

	love.graphics.setColor(255,100,160,255)
	love.graphics.print(" fps and delta " .. love.timer.getFPS() .. " " .. love.timer.getDelta(), 100, 100)
	love.graphics.setColor(255,255,255,255)

	love.graphics.rectangle("fill", 300, 300, 32, 32)
	love.graphics.rectangle("line", 420, 300, 64, 32)

	love.graphics.setColor(math.random(0,255),math.random(0,255),math.random(0,255))
	love.graphics.circle("fill", 200, 200, 16, 12)
	love.graphics.circle("fill", 360, 200, 32, 7)
	love.graphics.setColor(255,255,255,255)

	love.graphics.circle("line", 270, 200, 64, 12)

	love.graphics.draw(image, 40, 40, 125)
	love.graphics.draw(image, 420, 10, 0, 2, 2)
end

function love.update(dt)
	if love.keyboard.isDown("d","right","a","left","w","up") then
		print("d or right or a or left or w or up has been pressed :O")
	end
end

function love.wheelmoved(y)
	print("Wheel has been moved: " .. y)
end
