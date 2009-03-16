template <class T>
class Grid3D
{
public:
	Grid3D() : sizex(0), sizey(0), sizez(0), data(NULL)
	{

	}
	~Grid3D()
	{
		if (data)
			delete [] data;
	}
	T &operator()(int x,int y,int z)
	{
		return data[x + y * sizex + z*(sizex*sizey)];
	}
	const T &operator()(int x,int y,int z) const
	{
		return data[x + y * sizex + z*(sizex*sizey)];
	}
	void resize(int newsx,int newsy,int newsz)
	{
		if (data)
			delete [] data;
		sizex=newsx;
		sizey=newsy;
		sizez=newsz;
		data = new T[sizex*sizey*sizez];
	}
	int sizex,sizey,sizez;
	T *data;
};