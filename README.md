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

- [헤더 파일들](https://github.com/moad6127/TopDownRPG/tree/master/Aura/Source/Aura/Public/AbilitySystem/Abilities)
- [CPP 파일들](https://github.com/moad6127/TopDownRPG/tree/master/Aura/Source/Aura/Private/AbilitySystem/Abilities)

> GameplayAbility는 GAS를 사용하는 Actor가 프로젝트에서 수행할수 있는 행동이나 기술을 모두 포함하는 능력이다.
비동기적으로 사용되며 특정한 시점에서 활성화되면 Task를 진행하며 필요한 작업을 하게 된다.
GAS에서는 능력에 필요한 리소스를 사용하게하는 Cost와 Cooldown이 포함되어 있어서 비교적 쉽게 이것들을 사용할수 있게 만들수 있다.

> Ability를 사용하기 위해서는 AbilitySystemComponen에 해당 Ability를 부여한다음 활성화 단계를 거쳐야 하며 현재 프로젝트는 부여를 하자마자 바로 활성화 하는 Passive능력이 있으며 특정 시점에 사용할수 있는 Offensive능력이 있다. 또한 Damage를 주거나 받는 Ability와 XP를 획득하는 Non타입의 Ability도 존재한다.

> Ability에는 Tag가 존재하며 Tag가 들어오면 Tag를 확인하여 해당 Tag에 맞는 Ability를 실행 시킬수 있으며,
Tag를 통해 현재 Ability가 어떤 Tag를 가지고 있고 Sorce와 Target에 Tag를 부여할수도 있으며 특정 Tag가 존재할경우 해당능력을 사용하지 못하게 할 수도 있다.

### ProjectAbiliteis

> 현재 프로젝트에서는 Player가 사용하는 능력들과 Enemy들이 사용하는 공격 능력등이 있다.
Player들은 Offensive능력과 Passive능력을 가지고 있으며 Enemy를 처리할때 획득할 XP를 증가시키는 Ability도 존재한다.
 
![ArcaneShardAbility](https://github.com/user-attachments/assets/4be4f35a-4256-4ff6-b2c8-31902bfef880)
> Player의 Ability중 하나인 ArcaneShard공격이다.

## AbilityCost

> GameplayAbility는 Cooldown과 게임에서 리소스로 사용할것을 지불하는 Cost능력이 기본적으로 존재한다.
이것들 또한 GameplayEffect를 사용해서 GameplayAbility에 등록시키고 GameplayAbility내부의 Commit함수를 사용해 Cooldown과 cost를 지불하게 만든다.

![GameplayAbility_CooldownAndCost](https://github.com/user-attachments/assets/a64b350e-3741-4551-b47c-3bcf4a2acaef)

> GameplayAbility의 블루프린트에서 Commit함수를 사용하는 모습이다. CommitAbility를 사용하면 Cost와 Cooldown을 모두 사용하며 각각 따로 사용하는 CommitAbilityCooldown과 CommitAbilityCost함수가 존재한다.


<img src="https://github.com/user-attachments/assets/befaa399-e9a2-401d-a602-158805fea8f2">
  
>능력을 사용하면 정해준 Cooldown시간이 있으며 시간동안에 능력을 다시 사용할수 없게 만든다.


![GameplayAbility_CostEffect](https://github.com/user-attachments/assets/21beea95-7f30-4129-a2a1-12da124b62df)

> Cost를 사용하기위해 GameplayEffect를 만들고 Effect를 설정한 모습이다. Cost도 결국 프로젝트의 Attribute의 Mana를 사용하기때문에 Effect를 만들고 사용하게 된다.

## AbilityCooldown
![GameplayAbility_CooldownTag](https://github.com/user-attachments/assets/8f0f05d8-018b-4ca1-ab50-671d09464f7b)

> Cooldown을 사용하기 위해서는 CooldownTag가 존재해야 하며 CooldownEffect블루프린트에서 Tag를 설정해야 한다.

![GameplayAbility_CooldownEffect](https://github.com/user-attachments/assets/294c261a-fe93-4959-a1fd-18f6fd2e5a8a)

> Cooldown의 Effect설정이다.
> Has Duration으로 설정하며 Cooldown시간만큼 Float을 설정해서 사용한다.

![GameplayAbility_SetCooldownAndCost](https://github.com/user-attachments/assets/27578c17-329d-4aa1-bae1-8913a9375704)

> 에디터의 GameplayAbility의 블루프린트에서 CooldownEffect와 CostEffect를 설정한 모습이다.
> Cooldown과 cost를 설정한후 Commit함수를 사용하면 간단하게 Cooldown과 cost기능을 사용할수 있게 된다.


------------------------------------------------------------------------------------------------------


## *GameplayCue*

> GameplayCue는 능력에 사용되는 Sound나 VFX등의 연출을 화면에 보여주게 만들어주는 기능으로 기본적으로 Replicate를 지원하기 때문에 멀티플레이를 사용할때 Client의 화면에도 보여줄수 있다.

> GameplayCue를 사용할때 필요한 Parameter를 Input으로 넣어주면 Cue의 동작에 필요한 것들을 획득할수 있으며 이러한 Parameter를 활용해서 어느위치에 어떤 효과를 사용할지 만들수 있다.

----------------------------------------------------------------------------------------------------


# *GameplayTag*

> GameplayTag는 GAS를 사용할때 필수적으로 사용되지만, GAS가 아닌 프로젝트에서도 사용할 수 있다.   
GameplayTag는 계층적으로 존재하며 각 계층구조는 점으로 구분되는 부모 자식의 관계로 있다.   
Tag는 코드를 유연하게 만드는 도구이며 Match함수를 사용해서 Tag가 같은지 볼수 있으며 같은 부모를 가진 Tag를 확인할수도 있다.    
GAS에서는 거의 모든 클래스에서 GameplayTag가 사용되며 액터에게 Tag를 부여하는 개념이 존재한다.    
즉, AbilitySystemComponent에는 Tag를 부여하거나 확인할수 있는 Interface가 기본적으로 존재하며 해당 Interface함수를 사용해서 Tag를 확인하거나 가져올수 있다.   
프로젝트에 존재하는 Input, Ability, Attribute, DamageType, Buff/Debuff등등 모든 것들을 Tag로 만들어서 필요할때 사용가능하게 만들었다.   

- [헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/TopDownRPGGameplayTags.h)
- [C++ 파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/TopDownRPGGameplayTags.cpp)

<img src="https://github.com/user-attachments/assets/b1f49723-6abe-4f70-971d-66cb7e90d9a9" width=70% height=70%>

> GameplayTag는 에디터의 프로젝트 세팅이나, DataTable등을 사용해서 만들수 있지만, 이번 프로젝트에서는 Native방식으로 C++의 코드에서 사용이 가능하도록 Singleton방식으로 만들었다.

![AssetManager_Init](https://github.com/user-attachments/assets/1b82e5f5-fc6d-45aa-9ae6-6b71c11092cd)
>AssetManager를 사용해서 GaemplayTag의 초기화를 사용하면 C++뿐 아니라 에디터에서도 사용이 가능해진다.

---------------------------------------------------------------------------------------------

# *UI*

![UI_main](https://github.com/user-attachments/assets/35513ddd-d330-4e5a-884b-e7daae95fa52)

- [UI헤더 파일들](https://github.com/moad6127/TopDownRPG/tree/master/Aura/Source/Aura/Public/UI)
- [UI C++ 파일들](https://github.com/moad6127/TopDownRPG/tree/master/Aura/Source/Aura/Private/UI)

> 해당 프로젝트의 UI는 GAS를 사용하여 데이터를 주고받아야 하기때문에 화면에 보여지는 View와 GAS의 Data를 한번에 연결하지 않고, View와 Data사이에 Controller를 두는 MVC아키텍쳐 모델을 사용한다

- [HUD 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/HUD/TopDownRPGHUD.h)
- [HUD C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/HUD/TopDownRPGHUD.cpp)

## *Widget*

- [헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/Widget/TopDownRPGUserWidget.h)
- [C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/Widget/TopDownRPGUserWidget.cpp)

> 이번 프로젝트에서 Overlay와 AttributeMenu,SpellMenu에 사용된 Widget은 전부 해당 Widget클래스를 부모로 블루프린트로 만들어져서 사용된다.

<img src="https://github.com/user-attachments/assets/47e23b1a-8aa5-4462-8c30-3a08d7fa5e64" width=70% height=70%>
<img src="https://github.com/user-attachments/assets/1eeb326e-dd3c-4957-b7d1-3717896f0160" width=70% height=70%>

> 해당 클래스는 Controller변수만 담고있고, Setter를 사용해서 Widget에 사용되는 Controller를 설정하는 기능만 존재하며,
> BlueprintImplementableEvent UFUNCTION을 사용해서 WidgetController가 Set되면 블루프린트 이벤트가 호출되도록 만들었다.


## Overlay 

<img src="https://github.com/user-attachments/assets/71395aa9-31ba-4820-bc81-7b43493663e5" width=70% height=70%>

- [OverlayController 헤더](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/WidgetController/OverlayWidgetController.h)
- [OverlayController C++](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/WidgetController/OverlayWidgetController.cpp)

> HUD를 통해서 Player의 화면에 보여주는 UI로 기본적인 HealthGlobe, ManaGlobe 그리고 XP와 PlayerLevel도 표시하며 Player가 SpellAbility를 장착할경우 화면에 표시되게 만든다.
> AttributeMenu와 SpellMenu가 존재하여 해당 버튼을 클릭할경우 Attribute와 SpellMenu를 화면에 표시할수 있다.

![UI_Overlay_Delegates](https://github.com/user-attachments/assets/4447361d-a67d-4620-8ed3-216c2ef16641)

> 델리게이트를 통해서 변화가 감지되면 Overlay에 필요한 정보들을 최신화 한다.
> Health와 Mana, XP,PlayerLevel등의 델리게이트가 있으며 값이 변하면 델리게이트를 통해서 Overlay에 표시하게 된다.


<br/> <br>

![UI_Character_Init](https://github.com/user-attachments/assets/263e8d77-2e30-42a9-ba12-2b2da221fcaf)
![UI_HUD_InitOverlay](https://github.com/user-attachments/assets/60a7c735-084f-484f-b256-99a0ff5fbbba)

> Character클래스에서 필요한 정보들(GAS의 정보들)을 HUD클래스에 넘겨주면서 초기화 한후 화면에 표시한다.

![UI_Overlay_WidgetControllerSet](https://github.com/user-attachments/assets/e1e1a5c3-14d2-4e9e-bf86-060fbfb65521)
![UI_Overlay_SetController](https://github.com/user-attachments/assets/8474b087-7093-4d5b-92cf-04edb9456068)

> 블루프린트에서 WidgetControllerSet이벤트를 발생시켜서 해당 이벤트가 실행될때 실행되는 작업들이다.
> Overlay의 SubWidget에 각각 WidgetController를 부착시켜준후 필요한 정보들을 넘겨준다.

![UI_Overlay_construct](https://github.com/user-attachments/assets/b3a8af86-02eb-4d95-bc72-c57d3f21176e)

> OverlayWidget의 블루프린트에서 Construct이벤트가 진행될때 AttributeMenu버튼과 SpellMenu버튼을 각각 AttributeMenu와 SpellMenu가 열리도록 설정해둔다.
 

## AttributeMenu


![UI_AttributreMenu](https://github.com/user-attachments/assets/b2c4edb3-1adf-42ad-b33d-20cefad1f21a)

- [AttributeMenuWidgetController 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/WidgetController/AttributeMenuWidgetController.h)
- [AttributeMenuWidgetController  C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/WidgetController/AttributeMenuWidgetController.cpp)

> Player의 Level이 증가할때 일정한 수의 Point를 주어서 Player의 Attribute를 올릴수 있게 만든것으로 각각의 SubWidget들을 모아서 하나의 AttributeMenu를 만들었다.
> Point가 존재할경우 +버튼을 누르면 Attribute가 증가하도록 만들고 PrimaryAttribute가 증가하면 SecondaryAttribute도 증가하게 되어있어 자동으로 표시한다.


### *Init*

![UI_AttributeMenu_Const](https://github.com/user-attachments/assets/3788f389-e9ea-4949-9dfe-735c6b58ad24)
<img src="https://github.com/user-attachments/assets/fdc24c0b-3baf-4681-9a9a-1621f6bfd57c" width=70% height=70%>
<img src="https://github.com/user-attachments/assets/91d93a60-4fd5-4020-930e-f1617863b959" width=70% height=70%>


> AttributeMenu를 열면 각각의 필요한 작업들(Controller세팅, Tag지정)등을 한후 Attribute의 값에 대한 초기화를 진행한다.

<img src="https://github.com/user-attachments/assets/d1c788ce-9b5b-42dd-b996-77fe9ca8024e" width=70% height=50%>
<img src="https://github.com/user-attachments/assets/bc2aa57e-3b90-4dea-874c-289f274db8e2" width=70% height=50%>

> 각각의 행들에 AttributeTag를 지정한후 해당 값에 대해 초기화가 진행된다.

### *UPGRADE*

![UI_AttributeMenu_Upgrade](https://github.com/user-attachments/assets/ca85ed4e-413b-464b-8913-1facc079058c)
![UI_AbilitySystem_ServerUpgrade](https://github.com/user-attachments/assets/3fd7dee5-bcf9-4598-9baa-0b4c798a3791)

> Player의 XP가 차서 Levelup을 하게되면 일정량의 Point를 얻게 된다. 해당 포인트를 사용해서 PrimaryAttribute의 값을 증가시킬수 있다.
> AttributeMenu의 버튼을 누르면 Upgrade함수가 호출되고 AbilitySystem의 UpgradeAttribute가 호출되며 ServerRPC함수를 통해 값을 증가시킨다.
> ServerRPC에서 SendGameplayEventToActor함수를 통해 올리려는 AttributeTag와 Event를 보내 GameplayEffect를 사용해 Attribute의 값이 증가된다.


## SpellMenu


  ![UI_SpellMenu](https://github.com/user-attachments/assets/9c1d3b8b-d058-47f3-a5c4-b1785303e43f)


- [SpellMenuController 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/WidgetController/SpellMenuWidgetController.h)
- [SpellMenuController C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/WidgetController/SpellMenuWidgetController.cpp)

> Player의 SpellAbility를 관리하는 SpellMenu이다. Offensive와 Passive tree로 나눠져 있으며,
>  Ability의 Damage,Cooldown,Cost등을 설명하는 Description창과 Spell을 장착하기 위한 Equipped창으로 구별되어 있다.
> OffensiveTree와 PassiveTree의 Globe에서는 각각 하나의 능력들이 Tag를 통해 연결되어 있으며, Player의 Level이 능력해제에 필요한 레벨에 도달하면 Point를 사용해 능력을 해제시킬수 있다.


### *UPGRADE*

![UI_SpellMenu_SpendPoint](https://github.com/user-attachments/assets/1f654005-8f4d-4bbb-9c93-f0464b689d96)
![UI_ASC_ServerSpendPoint](https://github.com/user-attachments/assets/f6329e0e-adac-41dd-a1e8-3dc955e5bfab)

> SpellPoint가 존재할 경우 SpellMenu의 SpendPoint버튼을 눌러 Ability를 해제하거나 레벨을 올릴수 있다.
> AbilitySystemComponent의 ServerPRC함수를 호출해 능력을 Upgrade할수 있으며, Ability의 Status에 따라서 Ability를 해금하거나, 레벨을 올린다.

![UI_SpellMenu_Status](https://github.com/user-attachments/assets/b98deba8-3fd9-4202-adeb-fb15b55d0a22)
> Ability들의 Status로 각각 Lock,Eligible,UnLock,Equip상태가 있다.

> Player의 Ability를 Upgarde한후 Status를 Update해주면 AbilityUpgrade가 끝이 난다.

<br/> <br>

### *EQUIPPED*

![UI_SpellMenu_GlobeSelect](https://github.com/user-attachments/assets/f807d73c-5f35-4da7-ad67-73c1713999de)
![UI_SpellMenu_GlobeSelect_2](https://github.com/user-attachments/assets/8158072b-bb37-4ee4-92fa-836fbe0018f6)

> Ability가 저장되어 있는 Globe를 클릭하게 되면 해당 함수가 호출되며 Description창 표시와 Equip버튼 활성화가 진행된다.

![UI_SpellMenu_ShouldEnableButton](https://github.com/user-attachments/assets/a27187e6-59cf-499e-bf3d-eb2902ab45a4)

> 만약 해당 Ability의 Status가 이상없고 SpellPoint가 존재하면 Equip버튼을 클릭할수 있게 된다.

![UI_EquipAnim](https://github.com/user-attachments/assets/ec9ef7a4-6555-413d-9e68-672283878afa)

> Equip버튼을 클릭하게 되면 WBP의 Animation이 작동하며 EquipSpellRow의 Offensive, Passive중 하나의 Globe를 선택해서 클릭할수 있게 된다.

![UI_SpellMenu_SpellRowGlobePress](https://github.com/user-attachments/assets/2c53a72d-0349-4e06-b87e-7330045a2f27)

> EquipSpellRow의 Globe를 클릭하게되면 해당 Ability의 Type이 맞는지 확인한후 AbilitySystemComponent의 ServerRPC함수인 EquipAbility함수를 호출하게 된다.

<br/> <br>
 
![UI_ServerEquip_1](https://github.com/user-attachments/assets/43963506-3f7d-44bb-87e7-953778f2ed93)
![UI_ServerEquip_2](https://github.com/user-attachments/assets/ceb7a4b7-2b08-4e25-a555-43f0525fee09)
![UI_ServerEquip_3](https://github.com/user-attachments/assets/132c6468-58f2-4b24-835d-d78a92a08be6)

> EquipSpellRow에 Ability를 장착하게 되면 Passive의 경우 바로 Ability 바로 활성화를 진행되며 Offensive의 경우 해당 Globe에 장착하게 된다.

<br/> <br>

<img src="https://github.com/user-attachments/assets/a8ff5e2c-ddb8-4310-bd1b-8d1b7899e4cc" width=70% height=50%>
<img src="https://github.com/user-attachments/assets/2509054e-c8c4-4a48-bd88-74791a4927a0" width=70% height=50%>

> Ability를 장착하게 되면 SpellMenu의 EquipRow창과 Overlay의 Ability창에 최신화를 위해 해당 함수들이 델리게이트를 통해서 호출되며 각각의 창에 Ability들이 장착된다.



---------------------------------------------------------------------------------------------

# *GameSave*

![Gamesave_LoadMenu](https://github.com/user-attachments/assets/74972981-7d2c-4e0d-a168-4525c72d522f)

> Player가 진행한 진행상황들을 Save하기 위해 SaveGame클래스를 사용해서 Game을 Save하는 기능을 사용한다.
> 이때 UI를 구성한 방법인 MVC모델이 아닌 MVVM아키텍쳐 모델을 사용해서 LoadMenu를 구성하였으며 SaveGame을 통해 Player의 레벨, XP, Attribute, Spell등을 저장하고 불러올수 있도록 만들었다.
> 또한 Map의 상태도 저장하여 Map의 진행상황도 알수 있게 하였다.

 ### *MVVM모델*

<img src="https://github.com/user-attachments/assets/e9f6f625-551b-4f81-8f36-882ac3dc4b75" width=70% height=70%>

- [LoadSlot 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/ViewModel/MVVM_LoadSlot.h)
- [LoadSlot C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/ViewModel/MVVM_LoadSlot.cpp)
<br/> <br>
- [LoadSlot 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/UI/ViewModel/MVVM_LoadScreen.h)
- [LoadScreen C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/UI/ViewModel/MVVM_LoadScreen.cpp)
  
> Unreal엔진의 ViewModel플러그인을 사용해서 MVVM모델을 사용할수 있게 된다. 아직 베타 버전이기 때문에 보완할점은 많이 있다.
> 하지만 MVVM플러그인을 사용하면 엔진에서 바인딩을 통해서 간단하게 Widget변수의 값들을 변경할수 있게 된다.



![GameSave_MVVM_FieldNotify](https://github.com/user-attachments/assets/68b24faf-d44e-4047-a90b-44a556c97c54)
![GameSave_MVVM_Cpp](https://github.com/user-attachments/assets/75a8bf32-2e7b-4710-9f48-c88c7f04681a)
<img src="https://github.com/user-attachments/assets/4fa0557c-6f4c-4a57-8e17-b116c5244c46" width=70% height=70%>

> MVVM모델에서 SourceCode로 FieldNotify를 하게되면 Source코드를 통해 값이 변경될때 에디터에서 쉽게 바인딩하여 화면의 값을 변경시킬수 있게 된다.


### *PlayerSave*

![GameSave_CheckPoint](https://github.com/user-attachments/assets/8a630367-e2ba-4c95-ae9c-5b1304715f07)


- [SaveGame 헤더파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Public/Game/LoadScreenSaveGame.h)
- [SaveGame C++파일](https://github.com/moad6127/TopDownRPG/blob/master/Aura/Source/Aura/Private/Game/LoadScreenSaveGame.cpp)

> Unreal의 SaveGame클래스를 사용해서 Player의 상태와 Attribute등을 저장한다.


<p align="center"> 
  <img src="https://github.com/user-attachments/assets/064247a9-f284-40e0-af9e-b9eda83df4c1" align="center" width="32%">
  <img src="https://github.com/user-attachments/assets/f0b10669-f3f8-492d-add3-e205e3450368" align="center" width="32%">
  <img src="https://github.com/user-attachments/assets/1b78db8c-11ae-43ad-8d3e-6927a4145c44" align="center" width="32%">
<figcaption align="center"></figcaption></p>

> SaveGame클래스에 Player의 저장할 정보들을 변수로 선언을 해둔다음 저장이 필요할때 해당 변수들의 값을 채워서 저장하게 된다.
> 현재 프로젝트는 3개의 Slot을 사용하여 게임을 저장하고 불러올수 있으며 LoadMenu에서 해당Slot을 선택해서 불러올수 있다.

![GameSave_Checkpoint_Overlap](https://github.com/user-attachments/assets/d09211f5-29ce-4ebe-af3c-1174348ab766)
![GameSave_Character_SaveProgress1](https://github.com/user-attachments/assets/f0d387fb-b51c-41d3-a859-052dfd213e0f)
![GameSave_Character_SaveProgress2](https://github.com/user-attachments/assets/e45f9e46-8f73-45c4-89f4-eb4ebf0e5534)

> Character가 CheckpointActor에 Overlap하게되면 SaveProgress가 작동하게 되며 게임이 처음에 지정한 Slot에 저장되게 된다.
> Character가 Load될 위치인 PlayrStartTag가 저장되며 Player의 Level,XP,Point들이 저장되고 Attribute중 PramaryAttribute가 저장된다.
> 이후 Player가 포인트를 사용한 Ability들이 Save파일에 저장이 된다.

### *WorldSave*


![GameSave_Checkpoint_Overlap](https://github.com/user-attachments/assets/0b176b59-93a4-4cc8-bdbc-99fa41c522d0)

> Checkpoint가 저장하게 될때 Player의 데이터뿐 아니라 World의 저장할 필요가 있는 Actor들도 저장하게 된다.
> GameMode의 SaveWorldState함수를 통해서 World에 저장이 필요한 액터들이 저장이 된다.

![GameSave_GameMode_SaveWorld](https://github.com/user-attachments/assets/89b9dedb-e8c6-4457-a7df-bc015c651c1a)
![GameSave_GameMode_SaveWorld2](https://github.com/user-attachments/assets/405499fa-79ed-401e-9c68-3c87fcf0ca5c)

> GameInstance를 통해 저장될 SlotName과 Index를 얻은후 SaveClass에 저장할 Map의 이름과 Map을 저장한다.
> 다음으로 해당 World에 있는 모든 Actor들을 확인하여 Save가 필요한 Actor일때 FMemoryWriter를 통해 Byte형식으로 만든후
> Serialize를 통해 SaveClass에 저장하게 된다.

### *PlayerLoad*

![GameSave_Character_LoadProgress](https://github.com/user-attachments/assets/e1b1638f-583c-40ab-a717-007ac37d5785)
![GameSave_Character_LoadProgress2](https://github.com/user-attachments/assets/76543c0d-72e7-4d57-b9a0-680e05d28e93)

> Player는 저장된 Data를 Load하여 이전에 Play한 Data를 사용할수 있다.
> 처음 Character를 만들었는지, 저장된 Data를 사용해야 하는지는 SaveGameClass의 bFirstTimeLoadIn변수를 통해서 결정되며 이전에 한번이라도 Save를 진행한 Data는 해당 변수가 True로 설정된다.
> SaveGameClass에 저장된 Ability, Level,XP,Point 그리고 Attribute를 Player에게 Load하게 된다.

![GameSave_CharacterAbilityLoad](https://github.com/user-attachments/assets/4d86992c-91fb-4133-b203-c62b5e84ef2c)

> SaveGame에 저장된 Ability들을 Load하는 함수이다.
> 단순히 저장된 Ability들을 Spec으로 만들고 Type에 따라서 바로 Active하는지와 단순히 Give를 하는지 판단하여 Ability를 Player에게 부여한다.

![GameSave_Character_AttributeLoad](https://github.com/user-attachments/assets/3f752345-cc08-4c25-9643-ab3eac4e3ad5)
![GameSave_Character_AttributeLoad2](https://github.com/user-attachments/assets/b986cc0a-ea0a-4c35-87b2-b9423e6ebf60)

> SaveGame에 저장된 Attribute를 Load하는 함수이다.
> SaveGame에 저장되는 Attirbute는 PrimaryAttribute 4종류만 저장되며 저장된 Attribute들을 SetByCaller형식으로 Playe에게 부여하게 된다.
> 부여된 PriamryAttribute를 바탕으로 SecondaryAttribute와 VitalAttribute를 수학적으로 계산해서 Character에게 적용하면 Attribute의 Load는 끝이 난다.

---------------------------------------------------------------------------------------------

### *World State Load*


