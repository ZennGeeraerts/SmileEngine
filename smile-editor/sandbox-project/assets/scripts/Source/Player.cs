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

        public bool v0;
        public char v1;
        public short v2;
        public int v3;
        public long v4;

        public byte v5;
        public ushort v6;
        public uint v7;
        public ulong v8;

        public double v9;
        public Vector3 V10;

        public float speed;
        public float time;
        public Entity other;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            transform = GetComponent<TransformComponent>();
            bool hasComponent = HasComponent<TransformComponent>();
            Console.WriteLine("{0}", hasComponent);

            rigidbody = GetComponent<RigidbodyComponent>();
            speed = 0;
        }

        public void OnUpdate(float deltaTime)
        {
            time += deltaTime;
            //float speed = 1.0f;
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

            Vector3 translation = transform.Translation;
            translation += velocity * deltaTime;
            transform.Translation = translation;
        }
    }
}
