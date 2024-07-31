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
  * [GameplayTag](#GameplayTag)
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

<img src="https://github.com/user-attachments/assets/2c44419b-7279-4b23-a47f-13527330995d" width=50% height=50%>
<img src="https://github.com/user-attachments/assets/681fe2fb-e6ee-489e-ab12-070032ac199d" width=70% height=70%>

>Accessors 메크로를 사용해서 Getter,Setter등을 한번에 만들어서 C++코드를 통해 사용할수 있게 만들수 있다.


>PrimaryAttribute 4종류의 변수를 코드로 나타내면 이런식이 된다.


<br/> <br>


![Aura_AttributeSet_PostGameplayEffectExecute](https://github.com/user-attachments/assets/a41b4bc8-b4dc-4cf2-81c2-4e3b0a2e980d)

>AttributeSet의 내부의 함수를 통해 Data가 들어오면 Attribute를 비교해서 해당 Attribute의 값을 변경시킬수 있다.
위의 스크린샷은 GameplayEffect를 통해서 Attribute의 변경을 위한 함수인PostGameplayEffectExecute함수로 Data를 받고 Attribute의 값을 조절한다.

> GameplayEffectExecute함수 말고도 AttributeChange함수를 통해서 Attribute가 변경될때의 시점을 확인해서 값을 변경시킬 수 있다.
각각의 함수들은 Post와 Pre의 함수들이 있으며 이름에 맞게 들어오기 전과 후의 값을 변경시킬수 있는 함수들이다.


<br/> <br>




----------------------------------------------------------------------------------------------------
## *GameplayEffec*

> GameplayEffect는 Attribute의 값들을 변경할때 사용되는 클래스 이다.
Modifre를 통해서 Attribute의 값들을 변경할수 있으며 종류로 Add, Multiply, Divide, Override가 있으며
>  Modifre Magnitude에 따라서 Scalable Float, Attribute Based, Custom Calculation Class(MMC), SetByCaller가 있다.

<br/> <br>
<br/> <br>

![Aura_AttributeSet_Pramary](https://github.com/user-attachments/assets/6f5dd4cc-11f0-4313-a251-a55d44f39a31)
> 위의 스크린샷처럼 필수적인Strength, Intelligence, Resilience, Vigor는 변수는  Modifre Magnitude의 Scalable Float을 사용해서 값을 직접넣거나 DataTable의 값을 지정해서 넣을수 있다.

<br/> <br>
![Aura_AttributeSet_Secndary](https://github.com/user-attachments/assets/02ec81b9-eb11-4071-911f-85d3f81a2a3b)
> SecondaryAttribute는 필수변수인 4가지의 Attribute를 Attribute Based방식으로 에디터에서 계산을 통해 값을 정해서 사용하게 만들었다.

<br/> <br>
<img src="https://github.com/user-attachments/assets/c4319aa4-b1c2-47fd-8b17-6baa1591eb97" width=70% height=70%>
> MaxHealth와 MaxMana는 Player의 Attribute뿐아니라 PlayerLevel도 값을 정하는데 추가하기 위해 MMC(ModMagnitudeCalculation)를 사용해서 Attribute의 값을 만들었다.

-----------------------------------------------------------------------------------------------------


## *GameplayAbility*


> GameplayAbility는 어떤기능들을 캡슐화하여 사용하는 클래스로
간단히 말해서 게임에서 플레이어나 Enemy등이 사용하는 능력들을 모두 GameplayAbility로 만들어진다.
GameplayAbiliy는 비동기식으로 AbilityTask를 통해 실행할수 있다.

------------------------------------------------------------------------------------------------------



## *GameplayCue*

----------------------------------------------------------------------------------------------------


# *GameplayTag*

> GameplayTag는 GAS시스템에서만 사용하는게 아닌 GAS프로젝트가 아닌 다른 프로젝트에서도 사용할수 있지만 프로젝트에 필요한 것들을 식별하는데 매우 유용하며 자주 사용된다.

---------------------------------------------------------------------------------------------

# *UI*

---------------------------------------------------------------------------------------------

# *GameSave*

---------------------------------------------------------------------------------------------


