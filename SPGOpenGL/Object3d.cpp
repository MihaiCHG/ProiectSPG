
namespace myGame {

	class Object3d {

	private:
		int x, y, z;

	public:
		Object3d(int _x, int _y, int _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		~Object3d() {};

		void DrawObject();

	};
}