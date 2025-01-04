# AAA_HealthSystem
*You can use this plugin with any version of Unreal Engine Just Download it and Build it .

The perfect solution for your Unreal Engine game development needs. This health system effortlessly adds a component to any actor and automatically triggers events on death and health changes. Implementing this system is super easy, saving you valuable development time. Our professional-grade health system ensures your players have an immersive and engaging gaming experience. Don't settle for less, upgrade your game development with the Reusable Health System for Unreal Engine. Get it now and take your game to the next level!

Multiplayer Ready .
*If you need any feature just ask me on Discord :- https://discord.gg/vxn4gadpwC

New Update V1.3:-
_Armor Added
_To Get Demo Project switch the branch to project and download it.

Features:

✅ Simple & Easy to Use

✅ Armor and Health will work in sync

✅ Add Health Component to Any Actor

✅ On Health Changed Event

✅ On MaxHealth Changed Event

✅ On Update HealthBar Event(every time health changes it gives a value b/w 0-1 to directly set in progress bar)

✅ On Update MaxHealthBar Event(every time maxhealth changes it gives a value b/w 0-1 to directly set in progress bar)

✅ Multiplayer Ready

✅Network Replicated: (Yes)(Check demo project(choose project branch))

*Full Access in Blueprints

![image](https://github.com/user-attachments/assets/10fd2d60-9289-40a3-b5ce-19eaafb87ff3)

*Health and Armor 

![image](https://github.com/user-attachments/assets/ab57210e-66f4-4f1b-ab3a-a8add924dffd)

⏬Step-by-Step-Guide:-----

Step 1. Install Plugin via Epic Launcher or Github

Using from Epic Launcher 🔽

![image](https://github.com/user-attachments/assets/6ea4dd78-4e81-4340-bd54-11c2ff3f4ff3)

Using from Github  🔽

![image](https://github.com/user-attachments/assets/5d267dcd-ba93-4d6a-85fa-919253bbcfd0)

Step 2. If you are using from Epic Launcher Skip it , if you are using from Github -> Make sure you have a c++ project if not create a c++ file and you are good to go , now create a "Plugins" folder in your project and add this download plugin here.

Using From Github 🔽

![image](https://github.com/user-attachments/assets/e5373a00-6cc4-457c-b3b9-adc142d7f623)

after this place your download plugin in this folder 

path will look something like this (D:\GitHub\AAA_HealthSystem\Plugins\AAA_HealthSystem)

![image](https://github.com/user-attachments/assets/433363f1-a58d-47af-b17e-8178624deb69)

Now Generate your project and build it.

Step 3. Add AAA_HealthSystem Component in your Actor Class for eg. :- your PlayerCharacter

![image](https://github.com/user-attachments/assets/8b58b6f7-8bc4-4a0d-b57a-6a4f589a65d5)

Step 4. Selct it and in detail panel you will find important settings for it.

![image](https://github.com/user-attachments/assets/7ae52056-ac29-4146-aa7b-ae25efa89e64)

Step 5. Modify Settings for Health

![image](https://github.com/user-attachments/assets/bb0dbf06-f83a-4079-8aea-e6f20a92e8da)

Health ➙ is your Current Health .

MaxHealth ➙ is your max health, we are using this value while regernating health , you current health will go up to this value, of couse you can modify this value later on if you need. 

HealthBarPercentage ➙ We are using this value to get progressbar value you can set progress value to 0-1 and use this to a value , for eg;- the health progress bar value will be Health/HealthBarPercentage (if health is 100 and HealthBarPercentage is 100 ➙ you will get 1 value means you have full health and on progress bar you can show this value)

Step 5. Modify Settings for Armor

![image](https://github.com/user-attachments/assets/8ffef254-9fad-43a3-a6c3-18a1a2cea7c2)

HasArmor ➙ Enable it To use Armor

ArmorDamageMultiplier ➙ Default Value is 1 (you can modify this value if you want to reduce Armor More Quickly or slowly)

RegenerateArmor ➙ Enable it if you want to regenerate armor like health (if Enabled , first it will generate health and then armor )

Step 6. Damage Actor

![image](https://github.com/user-attachments/assets/f6775b9b-2636-48c4-ba5a-83ede0acfd11)

you can use like this or you can write your own logic this will not affect anything

Step 7. Any Damage

![image](https://github.com/user-attachments/assets/342ef9ef-09b1-47b0-967b-27379dc464a1)

AnyDamage Event will let you know that this class get damaged

Now you can reduce your health (if you have Armor enabled it will damage armor first then it will damage your health)

Step 8. Regenerate Health

![image](https://github.com/user-attachments/assets/24313210-9b64-4109-b13c-70bc36dee018)

If you want to Regenerate your health you can do it if you are still alive or you can make your own logic for it. (if you have RegenerateArmor it will Regenerate Armor as well but after Health )

InTime ➙ in this interval this will run again and again until your current Health reaches equal to MaxHealth(and same for Armor if you have enabled RegenerateArmor).

HealthToRegin ➙ Amount you want to Regenerate in Given Time

Step 9. Event Dispachers (you can use these to set your progressbar values)

select you AAA_HealthSystem Component , in details panel you will find all these Events

![image](https://github.com/user-attachments/assets/e4d041ef-1f07-4769-a383-5fa91b2e124a)

You can use these 2 Events to Bind you Health and Armor ProgressBar

![image](https://github.com/user-attachments/assets/2f562a8c-df2e-4eee-9a49-5e90c8c2154e)


Step 10. Some more User Friendly Functions(in most cases ,their is no need to use these)

For Health

![image](https://github.com/user-attachments/assets/b29ac1c6-5be2-4606-85f2-0ca07388e8d6)

For Armor

![image](https://github.com/user-attachments/assets/46e031b1-9515-4af0-bd30-d17c6514cbf1)

Event Dispachers

![image](https://github.com/user-attachments/assets/e11e13e3-9a16-49df-b6b3-1c5b02f80c60)


You can found Demo Project on same repo , choose project branch and download it

![image](https://github.com/user-attachments/assets/f8b4780a-8058-429e-9d75-641d814a7534)

If you are Facing any issues feel free to DM me on Discord. 

If you want to contribute in this HealthSystem you are most welcome .


I will keep improving it with your feedback . 


