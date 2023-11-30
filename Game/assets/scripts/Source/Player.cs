using Sofia;
using System;

namespace Game
{
	public class Player : Entity
	{
		private TransformComponent m_Transform;

		public float Speed;
		public float Time = 0.0f;

		void OnCreate()
		{
			Console.WriteLine($"Player.OnCreate - {ID}");

			m_Transform = GetComponent<TransformComponent>();
		}
		void OnUpdate(double ts)
		{
			float speed = Speed;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			velocity *= speed * (float)ts;

			Vector3 position = m_Transform.Position;
			position += velocity * (float)ts;
			m_Transform.Position = position;
		}
	}
}
