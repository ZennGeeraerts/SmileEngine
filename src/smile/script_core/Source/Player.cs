using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Smile;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent transform;
        private RigidbodyComponent rigidbody;
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            transform = GetComponent<TransformComponent>();
            bool hasComponent = HasComponent<TransformComponent>();
            Console.WriteLine("{0}", hasComponent);

            rigidbody = GetComponent<RigidbodyComponent>();
        }

        public void OnUpdate(float deltaTime)
        {
            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyPressed(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyPressed(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyPressed(KeyCode.D))
                velocity.X = 1.0f;

            if (Input.IsKeyPressed(KeyCode.Space))
            {
                rigidbody.AddForce(new Vector3(0.0f, 10.0f, 0.0f));
            }

            velocity *= speed;

            Vector3 _translation = transform.Translation;
            _translation += velocity * deltaTime;
            transform.Translation = _translation;
        }
    }
}
