// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.

#include "base/InputManager.hpp"
#include "base/GameObject.hpp"
#include "base/Level.hpp"
#include "base/GenericComponent.hpp"
#include "base/PatrolComponent.hpp"
#include "base/StatesAndTransitions.hpp"
#include "base/StateComponent.hpp"
#include "base/RemoveOnCollideComponent.hpp"
#include "base/RectRenderComponent.hpp"
#include "base/SDLGraphicsProgram.hpp"
#include <memory>



static const int TAG_PLAYER = 1;
static const int TAG_GOAL   = 2;
static const int TAG_BLOCK  = 3;
static const int TAG_ENEMY  = 4;



class JmpInputComponent: public GenericComponent {
public:

  JmpInputComponent(GameObject & gameObject, float speed, float jump, float gravity):
    GenericComponent(gameObject),
    mSpeed(speed),
    mJump(jump),
    mGravity(gravity)
  {
  }
  
  virtual void update(Level & level) override
  {
    bool left = InputManager::getInstance().isKeyDown(SDLK_LEFT);
    bool right = InputManager::getInstance().isKeyDown(SDLK_RIGHT);
    bool jump = InputManager::getInstance().isKeyPressed(SDLK_UP);
    
    GameObject & gameObject = getGameObject();
    std::shared_ptr<PhysicsComponent> pc = gameObject.physicsComponent();
    
    if (left && !right) {
      pc->setVx(-mSpeed);
    } else if (!left && right) {
      pc->setVx(mSpeed);
    } else {
      pc->setVx(0.0f);
    }

    if (jump) {
      bool onGround = false;
      std::vector<std::shared_ptr<GameObject>> objects;
      if (level.getCollisions(gameObject.x() + 0.0f * gameObject.w(), gameObject.y() + gameObject.h() + 2.0f, objects)) {
	for (auto obj: objects) {
	  if (obj->tag() == TAG_BLOCK) {
	    onGround = true;
	  }
	}
      }
      if (level.getCollisions(gameObject.x() + 1.0f * gameObject.w(), gameObject.y() + gameObject.h() + 2.0f, objects)) {
	for (auto obj: objects) {
	  if (obj->tag() == TAG_BLOCK) {
	    onGround = true;
	  }
	}
      }
      if (onGround) {
	pc->setVy(-mJump);
	return;
      }
    }
  
    pc->setVy(std::min(pc->vy() + mGravity, mJump));
  }

private:

  float mSpeed;
  float mJump;
  float mGravity;

};



const float SIZE = 40.0f;

class JmpPlayer: public GameObject {
public:
  JmpPlayer(float x, float y):
    GameObject(x, y, SIZE, SIZE, TAG_PLAYER)
  {
    addGenericCompenent(std::make_shared<JmpInputComponent>(*this, 10.0f, 20.0f, 1.0f));
    addGenericCompenent(std::make_shared<RemoveOnCollideComponent>(*this, TAG_GOAL));
    setPhysicsCompenent(std::make_shared<PhysicsComponent>(*this, false));
    setRenderCompenent(std::make_shared<RectRenderComponent>(*this, 0x00, 0xff, 0x00));
  }
};

class JmpGoal: public GameObject {
public:
  JmpGoal(float x, float y):
    GameObject(x, y, SIZE, SIZE, TAG_GOAL)
  {
    addGenericCompenent(std::make_shared<PatrolComponent>(*this, x - SIZE * 2.0f, y, 2.0f));
    setPhysicsCompenent(std::make_shared<PhysicsComponent>(*this, false));
    setRenderCompenent(std::make_shared<RectRenderComponent>(*this, 0xff, 0x99, 0x00));
  }
};

class JmpBlock: public GameObject {
public:
  JmpBlock(float x, float y):
    GameObject(x, y, SIZE, SIZE, TAG_BLOCK)
  {
    setPhysicsCompenent(std::make_shared<PhysicsComponent>(*this, true));
    setRenderCompenent(std::make_shared<RectRenderComponent>(*this, 0x99, 0x99, 0x99));
  }
};

class JmpEnemy: public GameObject {
public:
  JmpEnemy(float x, float y):
    GameObject(x, y, SIZE, SIZE, TAG_ENEMY)
  {
    // TODO PART 4: set up state machine
    const float speed = 3.0f;
    const float x1 = x + SIZE * 4, y1 = y;
    const float x2 = x, y2 = y + SIZE * 3;
    const float x3 = x, y3 = y;

    std::shared_ptr<StateComponent::State> moveState1 = std::make_shared<MoveState>(speed, x1, y1);
    std::shared_ptr<StateComponent::State> moveState2 = std::make_shared<MoveState>(speed, x2, y2);
    std::shared_ptr<StateComponent::State> moveState3 = std::make_shared<MoveState>(speed, x3, y3);

    std::shared_ptr<StateComponent> sc = std::make_shared<StateComponent>(*this);
    sc->setStartState(moveState1);
    sc->addTransition(moveState1, moveState2, std::make_shared<PointProximityTransition>(x1, y1, SIZE / 8.0f));
    sc->addTransition(moveState2, moveState3, std::make_shared<PointProximityTransition>(x2, y2, SIZE / 8.0f));
    sc->addTransition(moveState3, moveState1, std::make_shared<PointProximityTransition>(x3, y3, SIZE / 8.0f));
    addGenericCompenent(sc);

    addGenericCompenent(std::make_shared<RemoveOnCollideComponent>(*this, TAG_PLAYER));
    setPhysicsCompenent(std::make_shared<PhysicsComponent>(*this, false));
    setRenderCompenent(std::make_shared<RectRenderComponent>(*this, 0xdd, 0x22, 0x22));
  }
};



int main(int argc, char** argv) {
  std::shared_ptr<Level> level = std::make_shared<Level>(20 * SIZE, 20 * SIZE);

  level->addObject(std::make_shared<JmpPlayer>(2 * SIZE, 16 * SIZE));
  level->addObject(std::make_shared<JmpGoal>(18 * SIZE, 2 * SIZE));
  level->addObject(std::make_shared<JmpEnemy>(5 * SIZE, 5 * SIZE));
  for (int ii = 0; ii < 5; ++ ii) {
    level->addObject(std::make_shared<JmpBlock>((2 * ii + 5) * SIZE, (15 - 2 * ii) * SIZE));
  }
  for (int ii = 0; ii < 20; ++ ii) {
    level->addObject(std::make_shared<JmpBlock>(ii * SIZE, 19 * SIZE));
  }
  
  SDLGraphicsProgram mySDLGraphicsProgram(level);

  mySDLGraphicsProgram.loop();

  return 0;
}
