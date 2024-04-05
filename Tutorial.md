## Туториал к лаунчеру Black Russia
# Открытие проекта через Android Studio
Заранее разархивируем скачаный исходник в удобное вам место. 

![Нажмите на Open](https://cdn.discordapp.com/attachments/862764491624022037/1225885757961404416/image.jpg?ex=6622c214&is=66104d14&hm=0e33392380173a296c6149d74b6d3ffcf0db8c15f7b491b31ee6e1bcc39b0033&)

Открываем Android Studio. Там нажимаем open и выбираем папку с самим проектом.

![Выберите проект](https://cdn.discordapp.com/attachments/862764491624022037/1225886225668247682/image_1.jpg?ex=6622c284&is=66104d84&hm=eb26429900a96f15d650be67b702677810e72e96abedfcef276e7f10ff7568bc&)

Теперь выбираем место, куда вы разархивировали проект. Если у папки есть зелёный значёк Android, значит это то что нужно!

![Trust Project](https://cdn.discordapp.com/attachments/862764491624022037/1225886897935749121/image.png?ex=6622c324&is=66104e24&hm=403f0872665498dbe1e7f243e680af27c2a1311889844b6d97f5b4031890325f&)

Если появилось данное окно, нажимает Trust Project.
Проект должен бл открыться. Если у вас появилась ошибка:

![Error](https://cdn.discordapp.com/attachments/862764491624022037/1225814962476286013/image.png?ex=66228025&is=66100b25&hm=87af5b2ef9d70fdfecde1c9a81e49e15091c465168874c8a2e59525e3f8bfbe9&)

То просто наживаем на синий текст "Upgrade Gradle Wrapper"
Теперь ваш проект должен открыться, и должны появиться папки:

![Открытый проект](https://cdn.discordapp.com/attachments/862764491624022037/1225888188363706401/image.png?ex=6622c458&is=66104f58&hm=9002f2253a780143e382214bebe25eccc8c087e29c0ea2e7ac954bbfbacc42f2&)

# Подключение server.json и stories.json
Файлы [server.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/stories.json) отвечают за работу мониторинга серверов и историй в самом лаунчере:
![Меню лаунчера](https://cdn.discordapp.com/attachments/862764491624022037/1225881824492916867/image.png?ex=6622be6a&is=6610496a&hm=900a4b336306915b1056956a762865cbed168f553ada809d3afe15a01195fd58&)

Чтобы их подключить, их нужно изменить через любой текстовый редактор, и залить на облако и получить ***ПРЯМУЮ ССЫЛКУ!***
Для этого советую использовать [Dropbox](https://www.dropbox.com), так как там можно получить прямую ссылку если, вместо "0" в конце ссылки на файл написать "1"

Теперь в самом Android Studio переходим по пути app\java\com\blackrussia\launcher\other\Interface.java

![Interface.java](https://cdn.discordapp.com/attachments/862764491624022037/1225916719684522114/image.png?ex=6622deea&is=661069ea&hm=b43856123c7d2bdfa2a9fb160dd696a551a8b837582f373f3936a510f387655d&)

Как мы можем видеть, в данном коде имеются ссылки ведущие до файлов [server.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/stories.json).
За место этих ссылок, вставляете свои. Напоминаю что нужна именно ***ПРЯМАЯ ССЫЛКА!***
Всё готово!

# Настройка кэша (файлы игры) и его подключение.
Теперь давайте скачаем нужные файлы для игры. К сожалению я не смог их добавить на гитхаб так что залил его на Google Drive. Скачать - [тык*](https://drive.google.com/file/d/1_8SRrvfS3Mv2AgY0arntJtsJVONnFlR2/view?usp=sharing)
Откройте его через WinRar или другой архиватор. Перейдите в папку SAMP и откройте файл settings.ini

![settings.ini](https://cdn.discordapp.com/attachments/862764491624022037/1225938641042407424/image_1_1.jpg?ex=6622f354&is=66107e54&hm=c75124316b473eb0a3b022b38151be8728f45d6ce075fcb72994f4e412edbdd5&)

Ничего не советую менять, кроме ника который будет стоять сразу после установки, и максимальное значение фпс рекомендую поставить как у меня.
Всё, сохраняем кэш с перезаписью архива:

![WinRarSave](https://cdn.discordapp.com/attachments/862764491624022037/1225920357265838190/image.png?ex=6622e24d&is=66106d4d&hm=b5c0e430455814b9e22af394acb8737d41eb2c5f736277d9f4a75192710d445a&)

Теперь так же заливаете файл в облако. (прямая ссылка обязательна!)

Теперь как подключить кэш?
Для этого переходим по пути app\java\com\blackrussia\launcher\activity\LoaderActivity.java
И теперь меняете ссылку на свою:

![LoaderActivity.java](https://cdn.discordapp.com/attachments/862764491624022037/1225940218763546624/image.png?ex=6622f4cd&is=66107fcd&hm=7299727ac4b115f4c7f5be8f6409ecefa72ea04d9398d74429c14e3da699eab7&)

Готово!

# Изменение названия лаунчера
Изменить название лаунчера очень просто! Для этого нужно перейти в app\res\values\strings\strings.xml и поменять значение "app_name" на своё:

![String](https://cdn.discordapp.com/attachments/862764491624022037/1225880922864357438/image.png?ex=6622bd93&is=66104893&hm=394307d3e9efc405ea63961220af02eafad2d765adadc9c3a85da6a746b53351&)

Готово!

# Изменение иконки лаунчера
Поменять иконку лаунчера тоже довольно просто. Для этого вам нужна иконка 1060*1060 пикселей в формате .png
Теперь через проводник переходите по папкам и менете иконку на свою

BlackRussiaSource-main\app\src\main\res\drawable\xperiaicon.png

BlackRussiaSource-main\app\src\main\res\drawable\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-hdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-ldpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-mdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xhdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xxhdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xxxhdpi-v4\ic_launcher.png

![ic_launcher](https://cdn.discordapp.com/attachments/862764491624022037/1225882456524193885/image.png?ex=6622bf01&is=66104a01&hm=4db396ed6f243f81f7c43a18fa09e223150da795c823395d4232bdb7a49ce235&)

Но это заменена только иконка самого приложения, менять дизайн самого лаунчера нужно здесь:

BlackRussiaSource-main\app\src\main\res\drawable\

По этому пути находятся все изоображения используемые лаунчером, в том чисте и иконка лаунчера.

[drawable](https://cdn.discordapp.com/attachments/862764491624022037/1225944495015591998/image.png?ex=6622f8c8&is=661083c8&hm=e61e83dad2a23762d6bed373f199d1363e45d3fd6efa5a87f74c2fa62abf1e66&)

Готово!
______________________________________
Пока что туториал не закончен, но он дополняется... Скоро допишу :-)
