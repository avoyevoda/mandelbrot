#include "ComplexPlane.h"


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size.y = pixelHeight;
	m_pixel_size.x = pixelWidth;

	m_aspectRatio = pixelHeight / (pixelWidth * 1.0);

	m_plane_center.y = 0;
	m_plane_center.x = 0;

	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };

	m_zoomCount = 0;

	m_State = State::CALCULATING;

	m_vArray.setPrimitiveType(sf::Points);
	m_vArray.resize(pixelWidth * pixelHeight);

}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

//sus
void ComplexPlane::updateRender()
{
	Vector2f temp;
	Uint8 r, g, b;
	size_t count;

	if (m_State == State::CALCULATING)
	{
		for(int i = 0; i < m_pixel_size.y; i++)
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				//fill array with pixel screen size (height * width)
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
				// cast pixel screen location, returns plane location
				temp = mapPixelToCoords(Vector2i(j, i));
				// use plane locations to calculate iterations
				count = countIterations(temp);
				// iterations are passed to store color
				iterationsToRGB(count, r, g, b);
				// color is set
				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
	}
	m_State = State::DISPLAYING;

}
//sus
void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	m_plane_size.x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	
	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	m_plane_size.x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));

	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);
	m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	// consult image
	stringstream ss;
	ss << "Mandelbrot Set" << endl;
	ss << "Center: " << m_plane_center.x << ", " << m_plane_center.y << endl;
	ss << "Cursor: " << m_mouseLocation.x << ", " << m_mouseLocation.y << endl;
	ss << "Left click to Zoom in" << endl;
	ss << "Right click to Zoom out" << endl;

	text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y);
	complex<double> z(0, 0);
	int i;
	for (i = 0; i < MAX_ITER; i++)
	{
		z = z * z + c;
		if (abs(z) > 2)
			return i;
	}

	return i;
}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else if(count > 50)
	{
		r = 141;
		g = 25;
		b = 230;
	}
	else if (count > 30)
	{
		r = 87;
		g = 25;
		b = 230;
	}
	else if (count > 15)
	{
		r = 25;
		g = 124;
		b = 230;
	}
	else
	{
		r = 25;
		g = 216;
		b = 230;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	Vector2f temp;
	float ax, bx, ay, by;
	// x [0, 1920] y [1080, 0]
	ax = 0;
	bx = m_pixel_size.x;
	ay = m_pixel_size.y;
	by = 0;
	// m_plane_size
	// ((n - a) / (b - a)) * (d - c) + c
	//temp.x = ((mousePixel.x - ax) / (bx - ax)) * (m_plane_size.y - m_plane_size.x) + (m_plane_size.x);
	//temp.y = ((mousePixel.y - ay) / (by - ay)) * (m_plane_size.y - m_plane_size.x) + (m_plane_size.x);

	temp.x = ((mousePixel.x - ax) / (bx - ax)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	temp.y = ((mousePixel.y - ay) / (by - ay)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);
	if (mousePixel.x == 960 && mousePixel.y == 540)
	{
		cout << temp.x << " " << temp.y << endl;
	}
	//cout << "x formula: ((" << mousePixel.x << " - " << ax << ") / (" << bx << " - " << ax << ")) * (" << m_plane_size.x << ") + (" << m_plane_center.x << " - " << m_plane_size.x << " / 2.0)" << endl;

	return temp;
}


