# TopDownRPG

*GAS(GameplayAbilitySystem)을 사용해서 RPG게임을 만들어보기*

<br/> <br>

>이 프로젝트는 기본적인 RPG게임을 Unreal의 GAS를 활용해서 만든 프로젝트로
GAS를 사용해 Character의 속성(Attributes)과 능력(Ability)을 설정하고 능력에 필요한 리소스들과 능력의 Damage들을
속성을 통해 수학적으로 계산하며 사용할수 있도록 만들었다.


![Main](https://github.com/user-attachments/assets/85c72097-9694-4a92-a32d-e2517885a467)


## 목차
<details><summary> 열기/ 닫기</summary>
<p>
  
  * [AbilitySystem](#AbilitySystem)
    * [AbilitySystemComponent](#AbilitySystemComponent)
    * [AttributeSet](#AttributeSet)
    * [GameplayAbility](#GameplayAbility)
  * [UI](#UI)
  * [GameSave](#GameSave) 
  
</p>
</details>
<br/> <br>

# *AbilitySystem*

>GAS(GameAbilitySystem)은 언리얼엔진에서 만든 프레임 워크로 매우 유연하다.
캐릭터가 사용할 액션이나 패시브능력, 상태효과를 구축할수있고
이러한 효과의 결과로 다양한 속성들이 생기거나 닳게 만들수 있다
또한 이러한 효과를 조절하는 쿨타임과 능력에 필요한 리소스비용들을 설정해 능력을 사용할때마다 리소스를 사용하게 만들수 있다.(RPG게임의 Mana와 Cooldown같이)
각각의 능력들에 레벨을 넣거나 입자나 사운드같은 효과를 활성화 하는 작업도 편리하게 사용할수 있게 만들었다.


## *AbilitySystemComponent*


- [헤더파일 주소](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/AbilitySystem/TopDownRPGAbilitySystemComponent.h)
- [CPP파일 주소](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/AbilitySystem/TopDownRPGAbilitySystemComponent.cpp)


> AbilitySystemComponent로 액터에 추가할수 있는 컴포넌트로 GAS의 능력들을 사용하는데 필요한 모든 기능들을 담당하는 컴포넌트 이다 즉, GAS기능을 사용하기 원하는 액터라면 AbilitySystemComponent가 추가되어야 한다.
대표적으로 GAS기능을 사용할 Character들에게 Ability와 Attribute를 적용하고 장착하는 기능들을 가지고 있다.


>현재 프로젝트에서 Player가 Control하는 Character의 경우 PlayerState에 AbilitySystemComponet와 AttributeSet이 들어있으며 Enemy의 경우 Character클래스에 바로 포함되어 존재한다.

![Aura_AbilitySystemComponent_AddAbility](https://github.com/user-attachments/assets/d9d3798e-5891-4dab-92e0-cefb9eea7752)

> 위의 스크린샷처럼 Ability를 부여할수 있고 GameplayEffect로 Attribute를 추가해서 Attribute를 ApplyGameplayEffectSpecToTarget함수를 통해 부여할수 있다.


----------------------------------------------------------------------------------------------------


## *AttributeSet*

- [헤더파일 주소](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/AbilitySystem/TopDownRPGAttributeSet.h)
- [CPP파일 주소](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/AbilitySystem/TopDownRPGAttributeSet.cpp)

>게임에 주어진 개체나 캐릭터가 가지고있는 속성들로
게임에서 사용되는 체력이나 마나뿐아니라,
RPG에서 주로 사용되는 힘,민,체 같은 것들도 AttributeSet을 통해 Character에게 부여할수 있다.

----------------------------------------------------------------------------------------------------

## *GameplayAbility*


> GameplayAbility는 어떤기능들을 캡슐화하여 사용하는 클래스로
간단히 말해서 게임에서 플레이어나 Enemy등이 사용하는 능력들을 모두 GameplayAbility로 만들어진다.
GameplayAbiliy는 비동기식으로 AbilityTask를 통해 실행할수 있다.

------------------------------------------------------------------------------------------------------

## *GameplayEffec*

> GameplayEffect는 실행되면 속성의 값들을 변경하는데 사용되는 능력으로 속성과 관련된 반응들을 담당한다.


-----------------------------------------------------------------------------------------------------

## *GameplayCue*

----------------------------------------------------------------------------------------------------


# *GameplayTag*

> GameplayTag는 GAS시스템에서만 사용하는게 아닌 GAS프로젝트가 아닌 다른 프로젝트에서도 사용할수 있지만 프로젝트에 필요한 것들을 식별하는데 매우 유용하며 자주 사용된다.

---------------------------------------------------------------------------------------------

# *UI*




