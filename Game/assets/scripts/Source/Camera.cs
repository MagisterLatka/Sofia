using Sofia;
using System.Management.Instrumentation;

namespace Game
{
	public class Camera : Entity
	{
		public Entity OtherEntity;

		void OnUpdate(double ts)
		{
			float speed = 1.0f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.Up))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.Down))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.Left))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.Right))
				velocity.X = 1.0f;

			velocity *= speed;

			Vector3 position = Position;
			position += velocity * (float)ts;
			Position = position;
		}
	}
}
