#include "pfpch.h"

namespace Pitfall
{
    Player::Player(const String& name, const float2& spawnPos)
        : GameObject(name)
    {
        SetPosition(spawnPos);

        m_AABBCollider = new AABBCollider();
        m_AABBCollider->m_AABB.SetSize({ 1.0f, 2.0f });
        m_AABBCollider->SetCollisionLayer(PLAYER_COLLISION_LAYER);
        m_AABBCollider->SetCollisionIgnoreLayer(ENEMY_COLLISION_LAYER, true);



        m_Renderable = new Renderable();
        m_Renderable->m_SpriteRect.Size = { 2.0f, 2.0f };
        m_Renderable->m_ZIndex = 1;


        // Physics.
        m_RigidBody = new RigidBody();
        m_RigidBody->m_Drag = 4;
        m_RigidBody->m_Mass = 1;
        m_RigidBody->m_Gravity = { 0, -70};
        m_RigidBody->m_Friction = 30;

        m_DynamicHealth = Health(PLAYER_HP, PLAYER_DMG);
        m_RespawnPosition = GetPosition();

       // Animator initialization.
       m_Animator.SetSubject(m_Renderable);
       m_Animator.AddAnimation("Idle", SpriteLibrary::SpriteFromSurface("Player_Idle"));
       m_Animator.AddAnimation("Run", SpriteLibrary::SpriteFromSurface("Player_Run"));
       m_Animator.AddAnimation("Walk", SpriteLibrary::SpriteFromSurface("Player_Walk"));
       m_Animator.AddAnimation("Jump", SpriteLibrary::SpriteFromSurface("Player_Jump"));
       m_Animator.AddAnimation("Fall", SpriteLibrary::SpriteFromSurface("Player_Fall"));
       m_Animator.AddAnimation("Climb", SpriteLibrary::SpriteFromSurface("Player_Climb"));
       m_Animator.AddAnimation("Attack1", SpriteLibrary::SpriteFromSurface("Player_Attack1"));
       m_Animator.AddAnimation("Hurt", SpriteLibrary::SpriteFromSurface("Player_Hurt"));

        m_PixelCollider = new PixelCollider();
        m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));

       DynamicList<String> walkAssetNames(4);
       walkAssetNames[0] = "Assets/Audio/Player/Player_Walk_Grass0.wav";
       walkAssetNames[1] = "Assets/Audio/Player/Player_Walk_Grass1.wav";
       walkAssetNames[2] = "Assets/Audio/Player/Player_Walk_Grass2.wav";
       walkAssetNames[3] = "Assets/Audio/Player/Player_Walk_Grass3.wav";
       m_Audio.AddSoundBundle("Walk", walkAssetNames);

       DynamicList<String> hitAssetNames(4);
       hitAssetNames[0] = "Assets/Audio/Player/Player_Hit0.wav";
       hitAssetNames[1] = "Assets/Audio/Player/Player_Hit1.wav";
       hitAssetNames[2] = "Assets/Audio/Player/Player_Hit2.wav";
       hitAssetNames[3] = "Assets/Audio/Player/Player_Hit3.wav";
       m_Audio.AddSoundBundle("Hit", hitAssetNames);


       m_Audio.AddSound("Attack", "Assets/Audio/Player/Player_Attack.wav");
       m_Audio.AddSound("Climb", "Assets/Audio/Player/Player_Climb.wav");
       m_Audio.AddSound("Throw", "Assets/Audio/Player/Player_Throw.wav");
       m_Audio.AddSound("Jump", "Assets/Audio/Player/Player_Jump.wav");
    }

    void Player::Start()
    {
        m_ScoreDisplay = GameObject::FindByName<NumberDisplay>("ScoreDisplay");
        m_HealthDisplay = GameObject::FindByName<NumberDisplay>("HealthDisplay");

        m_Camera = GameObject::FindByType<Camera>();
        if(m_ScoreDisplay) m_ScoreDisplay->SetValue(m_Score);
        if(m_HealthDisplay) m_HealthDisplay->SetValue(m_DynamicHealth.HP);
    }

	void Player::Tick(float)
	{
		// Handle attack delay.
		if (m_Attacking)
		{
			if (m_AttackTimer.elapsed() > PLAYER_ATTACKTIME)
			{
				m_Attacking = false;
			}

            for (auto& e : GameObject::FindAllByType<Enemy>())
            {
                if (e->GetPixelCollider()->Collides(m_PixelCollider))
                {
                    e->Hit(m_DynamicHealth);
                    m_Attacking = false;
                }
            }
		}
		else
		{
			m_AttackTimer.reset();
		}

        m_Audio.Tick();
		m_Animator.Tick();

        // Movment.
        if (m_HasControls)
        {   
            if (m_PositionLastFrame.y > GetPosition().y + 0.01f)
            {
                m_Falling = true;
            }
            else
            {
                m_Falling = false;
            }

            m_Walking = false;

            // Update ground animations.
            if (!m_Jumped && !m_Falling)
            {
                const float absVelX = Math::Abs(m_RigidBody->m_Velocity.x);

                if (absVelX > 9.0f)
                {
                    m_Animator.SetAnimation("Run");
                    m_Animator.SetSpeed(absVelX);
                    m_Walking = true;
                }
                else if (absVelX > 2.0f)
                {
                    m_Animator.SetAnimation("Walk");
                    m_Animator.SetSpeed(absVelX * 1.5f);
                    m_Walking = true;
                }
                else
                {
                    m_Animator.SetAnimation("Idle");
                    m_Animator.SetSpeed(2.5f);
                }

                m_Audio.PlaySoundRepeating("Walk", PLAYER_AUDIO_WALKSPEED / absVelX);
            }
            else
            {
                m_Audio.Stop("Walk");
            }

            // Update inputs
            {
                if (Game::IsKeyDown(GLFW_KEY_SPACE) && !m_Jumped && !m_Falling)
                {
                    Jump(PLAYER_JUMPSTRENGTH);
                }

                if (Game::IsKeyPressed(GLFW_KEY_A))
                {
                    Move(-1);
                }

                if (Game::IsKeyPressed(GLFW_KEY_D))
                {
                    Move(1);
                }

                if (Game::IsKeyDown(GLFW_KEY_F))
                {
                    AttackHit();
                }
            }

            // Rock throwing
            if (Game::IsKeyDown(GLFW_KEY_R) && m_ThrowTimer.elapsed() > PLAYER_THROWTIME)
            {
                m_Audio.PlaySoundOneShot("Throw");
                m_ThrowTimer.reset();
                Instantiate(new Rock("Rock", this, GetPosition() + float2(0, 0.25f), float2(m_LookDirection * PLAYER_THROWSTRRENGTH, 0)));
            }

            // Update air animations.
            if (m_Falling)
            {
                m_Animator.SetAnimation("Jump", false);
                m_Animator.SetSpeed(3.0f);
            }

            // Set animation direction based on movement direction.
            if (m_RigidBody->m_Velocity.x < -0.5f || m_RigidBody->m_Velocity.x > 0.5f)
            {
                m_Animator.SetAnimationDirection(m_RigidBody->m_Velocity.x < 0 ? -1 : 1);
                m_PixelCollider->SetFlipped(m_RigidBody->m_Velocity.x < 0);
            }
        }
        else
        {
            m_Audio.Stop("Walk");
        }

        m_PositionLastFrame = GetPosition();

        // Debug mode.
#ifndef PF_DISTRIBUTE
        if (Game::IsKeyDown(GLFW_KEY_H))
        {
            m_Flying = !m_Flying;

            Print("Debug mode");
            PrintBool(m_Flying);

            if (m_Flying)
            {
                m_RigidBody->m_Gravity = { 0, -0 };
                m_AABBCollider->m_IsStatic = true;
                m_AABBCollider->m_IsTrigger = true;
            }
            else
            {
                m_RigidBody->m_Gravity = { 0, -70.0f };
                m_AABBCollider->m_IsStatic = false;
                m_AABBCollider->m_IsTrigger = false;
            }
        }

        if (m_Flying)
        {
            const int vel = Game::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ? 1 : 75;

            if (Game::IsKeyPressed(GLFW_KEY_UP))
            {
                SetPosition(GetPosition() + float2(0, vel * Game::DeltaTime()));
            }
            if (Game::IsKeyPressed(GLFW_KEY_DOWN))
            {
                SetPosition(GetPosition() + float2(0, -vel * Game::DeltaTime()));
            }
            if (Game::IsKeyPressed(GLFW_KEY_LEFT))
            {
                SetPosition(GetPosition() + float2(-vel * Game::DeltaTime(), 0));
            }
            if (Game::IsKeyPressed(GLFW_KEY_RIGHT))
            {
                SetPosition(GetPosition() + float2(vel * Game::DeltaTime(), 0));
            }
        }
#endif
    }

    void Player::Jump(float force)
    {
        m_RigidBody->m_Velocity.y += force;
        m_Jumped = true;
        m_Animator.SetAnimation("Jump", false);
        m_Audio.PlaySoundOneShot("Jump");
        m_Animator.SetSpeed(5.5f);
    }

    void Player::Move(int direction)
    {
        m_LookDirection = direction;
        m_RigidBody->m_Velocity.x += Game::DeltaTime() * PLAYER_MOVEMENTSPEED * (m_Falling ? 0.20f : 1.0f) * direction;
    }

    void Player::Halt()
    {
        m_RigidBody->m_Velocity = float2(0, 0);
    }

    void Player::Hit(const Health& entity)
    {
        m_DynamicHealth.HP -= entity.ATK;

        Print("Player HP Remaining: ");
        PrintInt(m_DynamicHealth.HP);

        m_Animator.SetAnimation("Hurt");
        m_Animator.SetSpeed(7.5f);
        m_Animator.OneShot();
        m_Audio.PlaySoundOneShot("Hit");
        m_Camera->PlayPixelationEffect();
        if (m_DynamicHealth.HP <= 0) Die();
        if (m_HealthDisplay) m_HealthDisplay->SetValue(m_DynamicHealth.HP);
    }

    void Player::Die()
    {
        SetPosition(m_RespawnPosition);
        m_DynamicHealth = Health(PLAYER_HP, PLAYER_DMG);
    }

    void Player::AddScore(int amount)
    {
        m_Score += amount;
        if (m_ScoreDisplay) m_ScoreDisplay->SetValue(m_Score);
    }

    void Player::RemoveScore(int amount)
    {
        m_Score -= amount;
        if (m_ScoreDisplay) m_ScoreDisplay->SetValue(m_Score);
    }

    void Player::AttackHit()
    {
        if (!m_Attacking)
        {
            // Play attack animation.
            m_Animator.Clear();
            m_Animator.SetAnimation("Attack1", false);
            m_Audio.PlaySoundOneShot("Attack");
            m_Animator.SetSpeed(15.0f);
            m_Attacking = true;
            m_Animator.OneShot();
        }
    }

    void Player::EnablePhysics(bool enabled)
    {
        m_RigidBody->m_EnablePhysics = enabled;
        Halt();
    }

    void Player::HandOverControls(bool handedOver)
    {
        m_HasControls = !handedOver;
    }

	void Player::OnCollision(const CollisionResult& result)
	{
        if (result.CollisionType == CollisionType::Pixel)
        {
            return;
        }

		if (!result.IsTrigger && result.Normal.y == 1)
		{
			m_Jumped = false;
		}

		// Rope swinging.
		SwingingRope* rope = TryCast<SwingingRope*>(result.Object);

		if (rope)
		{
			if (rope->Attached())
			{
				if (Game::IsKeyDown(GLFW_KEY_SPACE))
				{
					rope->Detach();
				}
			}
			else
			{
				rope->Attach();
			}
		}

		// Ziplining.
		Zipline* zipline = TryCast<Zipline*>(result.Object);

		if (zipline)
		{
			if (!zipline->Attached())
			{
				zipline->Attach();
			}
		}

        // Spike.
		if (result.CollisionType == CollisionType::Tile)
		{
            if (result.TileIndex == TILE_SPIKE)
            {
                m_RigidBody->m_Velocity.y = 40;
                Hit(Health(0, 5));
            }
		}

        AABBCollider::BasicOnCollision(this, result);
	}
}