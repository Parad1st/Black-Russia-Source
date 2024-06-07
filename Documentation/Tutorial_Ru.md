## Туториал к лаунчеру Black Russia
# Открытие проекта через Android Studio
Заранее разархивируем скачаный исходник в удобное вам место. 

![Нажмите на Open](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8C%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82.png)

Открываем Android Studio. Там нажимаем open и выбираем папку с самим проектом.

![Выберите проект](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/%D0%92%D1%8B%D0%B1%D0%BE%D1%80%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B0.png)

Теперь выбираем место, куда вы разархивировали проект. На операционной системе Windows у папки с проектом будет зелёный значёк Android, если это так, то значит это то что нужно!

![Trust Project](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/Trust%20project.png)

Если появилось данное окно, нажимает Trust Project.
Проект должен был открыться. Если у вас появилась ошибка:

![Error](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/error%20dradle%20qut.png)

Если появилась данная ошибка, то просто наживаем на синий текст "Upgrade Gradle wrapper"
Теперь ваш проект должен открыться, и должны появиться папки:

![Открытый проект](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8B%D0%B8%CC%86%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82.png)

# Подключение server.json и stories.json
Файлы [server.json](https://github.com/Parad1st/Black-Russia-Source/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/Black-Russia-Source/blob/main/Json%20files/stories.json) отвечают за работу мониторинга серверов и историй в самом лаунчере:
![Меню лаунчера](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/image%20(3).png)

Чтобы их подключить, их нужно изменить через любой текстовый редактор, и залить на облако и получить ***ПРЯМУЮ ССЫЛКУ!***
Для этого советую использовать [Dropbox](https://www.dropbox.com), так как там можно получить прямую ссылку если, вместо "0" в конце ссылки на файл написать "1"

Теперь в самом Android Studio переходим по пути app\java\com\blackrussia\launcher\other\Interface.java

![Interface.java](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/Interface%20%D1%84%D0%B0%D0%B8%CC%86%D0%BB.png)
Как мы можем видеть, в данном коде имеются ссылки ведущие до файлов [server.json](https://github.com/Parad1st/Black-Russia-Source/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/Black-Russia-Source/blob/main/Json%20files/stories.json).
За место этих ссылок, вставляете свои. Напоминаю что нужна именно ***ПРЯМАЯ ССЫЛКА!***
Всё готово!

# Настройка кэша (файлы игры) и его подключение.
Теперь давайте скачаем нужные файлы для игры. К сожалению я не смог их добавить на гитхаб так что залил его на Google Drive. Скачать - [тык*](https://drive.google.com/file/d/1_8SRrvfS3Mv2AgY0arntJtsJVONnFlR2/view?usp=sharing)
Откройте его через WinRar или другой архиватор. Перейдите в папку SAMP и откройте файл settings.ini

![settings.ini](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/settings.ini.png)

Ничего не советую менять, кроме ника который будет стоять сразу после установки, и максимальное значение фпс рекомендую поставить как у меня.
Всё, сохраняем кэш с перезаписью архива (скриншот из WinRar для Windows ):

![WinRarSave](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/Update%20settings.ini.png)

Теперь так же заливаете файл в облако. (прямая ссылка обязательна!)

Теперь как подключить кэш?
Для этого переходим по пути app\java\com\blackrussia\launcher\activity\LoaderActivity.java
И теперь меняете ссылку на свою:

![LoaderActivity.java](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/LoaderActivity.png)

Готово!

# Изменение названия лаунчера
Изменить название лаунчера очень просто! Для этого нужно перейти в app\res\values\strings\strings.xml и поменять значение "app_name" на своё:

![String](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/String%20app%20name.png)

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

![ic_launcher](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/ic%20launcher.png)

Но это заменена только иконка самого приложения, менять дизайн самого лаунчера нужно здесь:

BlackRussiaSource-main\app\src\main\res\drawable\

По этому пути находятся все изоображения используемые лаунчером, в том чисте и иконка лаунчера.

![drawable](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/drawable.png)

Готово!

# Изменение ip сервера и компиляция jni исходников
Чтобы заменить ip сервера на свой, мы должны открыть папку Jni source и открыть файл находящийся по пути              
Jni source/jni/util/CJavaWrapper.cpp

![CJavaWrapper.cpp](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/IPCJavaWrapper.png)

На 276 строчке вы увидете ip и порт сервера. Туда нужно вписать наш ip и порт сервера. (на фото показал где). После того как сделали это, сохраняем файл.       
Теперь нам нужно скомпилировать исходники, для этого переходим на данный сайт - [тык*](https://github.com/android/ndk/wiki/Unsupported-Downloads) и скачиваем NDK для нашей ос (32 битная версия не подходит)

![NDK Download](https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Documentation/Image/NDK%20Download.png)

Как компилировать на Windows показывал давно на YouTube - [тык*](https://www.youtube.com/watch?v=TSPvYpG15C0&t=74s). Если что туториал старый, и там не использовался Android Studio, но зато Jni исходники компилируются всё также!

ЭТО НЕ ВСЁ! СКОРО ДОПИШУ ТУТОР, ПОКА ЧТО ИГРА НЕ БУДЕТ КОННЕКТИТЬСЯ К СЕРВЕРУ, СКОРО ПОКАЖУ КАК ВСЁ СДЕЛАТЬ!
______________________________________
Пока что туториал не закончен, но он дополняется... Скоро допишу :-)
