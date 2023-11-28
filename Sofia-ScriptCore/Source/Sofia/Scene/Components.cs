namespace Sofia
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

	public class TransformComponent : Component
	{
		public Vector3 Position
		{
			get
			{
				InternalCalls.TransformComponent_GetPosition(Entity.ID, out Vector3 position);
				return position;
			}
			set
			{
				InternalCalls.TransformComponent_SetPosition(Entity.ID, ref value);
			}
		}
	}
}
