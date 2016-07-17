

--[[
--
--Read the pixels informations form an image and do stuff with it ;)
--
--]]


local t = {}
function spawn(x,y)
	table.insert(t,{x=x,y=y})
end

function love.load()
		
	local im = love.image.newImageData("map.png")
	
	for y = 1, im:getHeight() do
    for x = 1, im:getWidth() do
        -- Pixel coordinates range from 0 to image width - 1 / height - 1.
        local r, g, b, a, pixel= im:getPixel( x, y)
        if r == 55 and g == 50 and b == 227 then
        	  --print(x .. " " .. y)
        	  spawn(x,y)
        end
      end
	end

end

function love.update(dt)
	local mx = love.mouse.getX() 
	local my = love.mouse.getY()
	

	if love.keyboard.isDown("esc") then 
		love.event.quit()
	end

end

function love.draw()
	
	for i,v in ipairs(t) do 
		love.graphics.rectangle("fill",v.x*16,v.y*16,16,16)
	end
	
	love.graphics.print(""..love.timer.getFPS(),10,10)
end

function love.keypressed(k) 
	
end

function love.quit()
	
end

function love.mousepressed(x,y,b)
	
end

function love.wheelmoved(y)
	
end

