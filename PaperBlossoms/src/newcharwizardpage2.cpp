/*
 * *******************************************************************
 * This file is part of the Paper Blossoms application
 * (https://github.com/dashnine/PaperBlossoms).
 * Copyright (c) 2019 Kyle Hankins (dashnine)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Legend of the Five Rings Roleplaying Game is the creation
 * and property of Fantasy Flight Games.
 * *******************************************************************
 */

#include "newcharwizardpage2.h"
#include "ui_newcharwizardpage2.h"
#include <QDebug>
#include <QMessageBox>
#include "dictionary.h"

NewCharWizardPage2::NewCharWizardPage2(DataAccessLayer* dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage2)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle("Part 2: " + Dictionary::Common::Role + " and " + Dictionary::Common::School);
    ui->nc2_HIDDEN_skillLineEdit->setVisible(false); //holds a skill string

    //Handle special cases
    ui->nc2_kitsune_label->setVisible(false);
    ui->nc2_kitsune_comboBox->setVisible(false);
    QStringList kitsuneschoollist = dal->qsl_getschools(field("currentClan").toString(), true);
    kitsuneschoollist.removeAll(Dictionary::Schools::KitsuneImpersonatorTradition);
    ui->nc2_kitsune_comboBox->addItems(kitsuneschoollist);
    ui->nc2_kitsune_comboBox->setCurrentIndex(-1);

    settingupequip = false;

    //initialize models
    schoolModel   = new QStringListModel;
    skillOptModel = new QStringListModel;
    skillSelModel = new QStringListModel;
    ringModel     = new QStringListModel;

    //assign models to views
    ui->nc2_school_ComboBox->setModel(schoolModel);
    ui->nc2_skillOpt_listview->setModel(skillOptModel);
    ui->nc2_skillSel_listview->setModel(skillSelModel);
    ui->nc2_schoolSpecialtRing_ComboBox->setModel(ringModel);

    //populate model
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    ringModel->setStringList(dal->qsl_getrings());

    registerField("currentSchool",ui->nc2_school_ComboBox,"currentText");
    registerField("currentSchoolIndex*",ui->nc2_school_ComboBox);
    registerField("techChoices",ui->techWidget,"selections",SIGNAL(selectionsChanged)); //????? signal correct?
    registerField("equipSpecialChoices",ui->equipSpecialWidget,"selections",SIGNAL(selectionsChanged));
    registerField("equipChoices",ui->equipWidget,"selections",SIGNAL(selectionsChanged));
    registerField("ringChoices",ui->ringWidget,"selections",SIGNAL(selectionsChanged));
    //registerField("techstring*",ui->nc2_completePlaceholder_lineEdit);
    registerField("schoolSpecialRing",ui->nc2_schoolSpecialtRing_ComboBox,"currentText");
    registerField("schoolSpecialRingIndex*",ui->nc2_schoolSpecialtRing_ComboBox);
    registerField("schoolSkills*",ui->nc2_HIDDEN_skillLineEdit);
    registerField("q4Text",ui->nc2_q4_lineEdit);

    //ui->nc2_schoolDesc_textEdit->setVisible(false);

    connect(ui->equipWidget,SIGNAL(selectionsChanged(QString)), this, SLOT(equipSelectionChanged(QString)) );
    regenSummary();

}

NewCharWizardPage2::~NewCharWizardPage2()
{
    delete ui;
}

void NewCharWizardPage2::equipSelectionChanged(const QString newText){
    if(settingupequip) return;
    const QStringList specialCases = { //special cases
        Dictionary::EquipmentRaritySpecialCases::One_Weapon_of_Rarity_6_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::Two_Items_of_Rarity_4_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::Two_Weapons_of_Rarity_6_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::One_Sword_of_Rarity_7_or_Lower
    };
    ui->equipSpecialWidget->clear();

    const QStringList selected = ui->equipWidget->getCurrent();
    foreach (const QString box, selected) {
        const QString choice = box.split('|').at(1);
        if(specialCases.contains(choice)){    //if the 1-off is a special case
            handleSpecCases(choice);
        }

    }
    const QList<QStringList> equipsets = dal->ql_getlistsofeq(ui->nc2_school_ComboBox->currentText());              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()==2){                                //if the count is two, there's only one choice -- skip it
                QStringList choicesetforcombobox = equipsets.at(row);       //get the list of choices for the box
                choicesetforcombobox.removeFirst();                         //remove the count at the front
                if(specialCases.contains(choicesetforcombobox.first())){    //if the 1-off is a special case
                    handleSpecCases(choicesetforcombobox.first());
                }
            }
        }
    }
    qDebug() << "CAUGHT EQUIPSELECTIONCHANGED " <<newText;
}

void NewCharWizardPage2::handleSpecCases(QString speccase){
    const QString specCase = speccase;        //get thespecial case value
    if(specCase == Dictionary::EquipmentRaritySpecialCases::One_Weapon_of_Rarity_6_or_Lower){
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));
    }
    else if (specCase == Dictionary::EquipmentRaritySpecialCases::Two_Weapons_of_Rarity_6_or_Lower){
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));

    }
    else if (specCase == Dictionary::EquipmentRaritySpecialCases::Two_Items_of_Rarity_4_or_Lower) {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(4));
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(4));

    }
    else if (specCase == Dictionary::EquipmentRaritySpecialCases::One_Sword_of_Rarity_7_or_Lower) {
        ui->equipSpecialWidget->addCBox(dal->qsl_getweapontypeunderrarity(7, Dictionary::Weapons::Swords));

    }
}

void NewCharWizardPage2::initializePage(){

    const QString clan = field("currentClan").toString();
    qDebug()<< "Initializing page 2 with clan = " << clan;
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    ui->nc2_school_ComboBox->setCurrentIndex(-1);

    ui->nc2_unrestrictedSchool_checkBox->setChecked(false);
    regenSummary();


}

bool NewCharWizardPage2::validatePage(){

    if(skillSelModel->rowCount() <  dal->i_getschoolskillcount(ui->nc2_school_ComboBox->currentText())){
        QMessageBox msg;
        msg.setText("Error: insufficient " + Dictionary::Common::Skills.toLower() + " selected.");
        msg.exec();
        return false;
    }
    return true;
}

///////////// Allow schools outside of clan. /////////////////////////
void NewCharWizardPage2::on_nc2_unrestrictedSchool_checkBox_toggled(const bool checked)
{
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString(), checked));
}

//////////// Reconfigure page based on selected school //////////////////////
void NewCharWizardPage2::on_nc2_school_ComboBox_currentIndexChanged(const QString &arg1)
{
    //handle special cases
    if(arg1 == Dictionary::Schools::KitsuneImpersonatorTradition){
        ui->nc2_kitsune_label->setVisible(true);
        ui->nc2_kitsune_comboBox->setVisible(true);
        ui->nc2_kitsune_comboBox->setCurrentIndex(0);
    }
    else{
        ui->nc2_kitsune_label->setVisible(false);
        ui->nc2_kitsune_comboBox->setVisible(false);
        ui->nc2_kitsune_comboBox->setCurrentIndex(-1);
    }

    //set the desc
    ui->nc2_schoolDesc_textEdit->setText(dal->qs_getschooldesc(arg1)+ " " + dal->qs_getschoolref(arg1));

    qDebug() << "School changed to:  " + arg1;
    //ui->nc2_schooldesc_textEdit->setText(dal->qs_getschooldesc(arg1));
    const int skillcount = dal->i_getschoolskillcount(arg1);
    qDebug()<< skillcount;
    const QString skilllabel = "Choose " + QString::number(skillcount) + " " + Dictionary::Common::Skills.toLower() + ":";
    ui->nc2_skill_Label->setText(skilllabel);
    skillOptModel->setStringList(dal->qsl_getschoolskills(arg1)); //set list with school contents
    skillSelModel->setStringList( QStringList{} );  //clear prior selections, since this changed

    //TECHNIQUES//
    ui->techWidget->clear();
    const QList<QStringList> techsets = dal->ql_getlistsoftech(arg1);
    for(int row = 0; row < techsets.count(); ++row){
        int choosenum = techsets.at(row).at(0).toInt();
        for(int boxcount = 0; boxcount < choosenum;++boxcount){
            QStringList choicesetforcombobox = techsets.at(row);
            choicesetforcombobox.removeFirst();
            qDebug() << "Adding Box: " << choicesetforcombobox ;
            ui->techWidget->addCBox(choicesetforcombobox, "Choose a " + Dictionary::Common::Technique.toLower() + ":");
        }
    }

    //EQUIP//

    QString schoolname = arg1;
    if(arg1 == Dictionary::Schools::KitsuneImpersonatorTradition) schoolname = ui->nc2_kitsune_comboBox->currentText();
    doEquip(schoolname);

    //RINGS//
    ui->ringWidget->clear();
    const QStringList schoolrings = dal->qsl_getschoolrings(arg1);
    foreach(const QString ring, schoolrings){

        QStringList choicesetforcombobox;

        if(ring == "any"){
            choicesetforcombobox = dal->qsl_getrings();
        }
        else{
            choicesetforcombobox.append( ring);
        }
        qDebug() << "Adding Box: " << choicesetforcombobox ;
        ui->ringWidget->addCBox(choicesetforcombobox, Dictionary::Common::Ring + ":");

    }
    //this->resize(sizeHint());
    this->adjustSize();
    this->window()->adjustSize();
    regenSummary();

}


void NewCharWizardPage2::doEquip(const QString school){
    const QString schoolname = school;

    settingupequip = true;
    ui->equipSpecialWidget->clear();
    ui->equipWidget->clear();
    const QStringList specialCases = {
        Dictionary::EquipmentRaritySpecialCases::One_Weapon_of_Rarity_6_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::Two_Items_of_Rarity_4_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::Two_Weapons_of_Rarity_6_or_Lower,
        Dictionary::EquipmentRaritySpecialCases::One_Sword_of_Rarity_7_or_Lower
    };
    const QList<QStringList> equipsets = dal->ql_getlistsofeq(schoolname);              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()>2){                                //if the count is two, there's only one choice -- skip it
                int choosenum = equipsets.at(row).at(0).toInt();            //the first number is how many you can choose
                for(int boxcount = 0; boxcount < choosenum;++boxcount){     //otherwise, add a box for each count you get to choose
                    QStringList choicesetforcombobox = equipsets.at(row);   //get the list of choices for the box
                    choicesetforcombobox.removeFirst();                     //remove the count at the front

                    qDebug() << "Adding Box: " << choicesetforcombobox ;
                    ui->equipWidget->addCBox(choicesetforcombobox, "Choose an " + Dictionary::Common::Item.toLower() + ":");         //add the combobox
                }
            }
            else{                                                           //if it's 1, handle only if it's multiitem
                QStringList choicesetforcombobox = equipsets.at(row);       //get the list of choices for the box
                choicesetforcombobox.removeFirst();                         //remove the count at the front
                if(specialCases.contains(choicesetforcombobox.first())){    //if the 1-off is a special case
                    handleSpecCases(choicesetforcombobox.first());
                }
            }
        }
    }
    settingupequip = false;

}

/////////////// ADD and REMOVE Skills ///////////////////////////////
void NewCharWizardPage2::on_nc2_skillAdd_pushButton_clicked()
{
    doAddSkill();
}

void NewCharWizardPage2::doAddSkill(){
    const QModelIndex index = ui->nc2_skillOpt_listview->currentIndex();
    const QString itemText = index.data(Qt::DisplayRole).toString();
    if(itemText.isEmpty()) return;
    qDebug()<<itemText;
    QStringList sellist = skillSelModel->stringList();
    if (!sellist.contains(itemText) && sellist.count() < dal->i_getschoolskillcount(ui->nc2_school_ComboBox->currentText())){
        sellist.append(itemText);
        skillSelModel->setStringList(sellist);
    }
    QString skillstring = "";
    foreach(QString skill, skillSelModel->stringList()){
        skillstring += skill + "|";
    }
    if (skillstring.length()>=1)skillstring.chop(1); //remove trailing separator
    ui->nc2_HIDDEN_skillLineEdit->setText(skillstring);
    regenSummary();

}

void NewCharWizardPage2::on_nc2_skillRem_pushButton_clicked()
{
   doRemSkill();

}

void NewCharWizardPage2::doRemSkill(){
    const QModelIndex index = ui->nc2_skillSel_listview->currentIndex();
    const QString itemText = index.data(Qt::DisplayRole).toString();
    qDebug()<<itemText;
    QStringList sellist = skillSelModel->stringList();
    if (sellist.contains(itemText)){
        sellist.removeAll(itemText);
        skillSelModel->setStringList(sellist);
    }
    QString skillstring = "";
    foreach(QString skill, skillSelModel->stringList()){
        skillstring += skill + "|";
    }
    if (skillstring.length()>=1)skillstring.chop(1); //remove trailing separator
    ui->nc2_HIDDEN_skillLineEdit->setText(skillstring);
    regenSummary();

}

void NewCharWizardPage2::on_nc2_kitsune_comboBox_currentTextChanged(const QString &arg1)
{
    if(!ui->nc2_school_ComboBox->currentText().isEmpty())
        doEquip(arg1);
}

void NewCharWizardPage2::on_nc2_skillOpt_listview_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    doAddSkill();
}

void NewCharWizardPage2::on_nc2_skillSel_listview_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    doRemSkill();

}


void NewCharWizardPage2::regenSummary(){
   QString skills = "";
   QString rings = "";

   const QMap<QString, int> ringmap = calcCurrentRings();

   QMapIterator<QString, int> i(ringmap);
   while (i.hasNext()) {
       i.next();
       if(!i.key().isEmpty()){
        rings+="  "+i.key()+": "+QString::number(i.value())+ "\n";
       }
   }

   const QMap<QString, int> skillmap = calcSkills();

   QMapIterator<QString, int> si(skillmap);
   while (si.hasNext()) {
       si.next();
       if(!si.key().isEmpty()){
        skills+="  "+si.key()+": "+QString::number(si.value())+ "\n";
       }
   }

    ui->summary_label->setText(Dictionary::Common::Rings + ":\n"+rings+"\n\n" + Dictionary::Common::Skills + ":\n"+skills);

}

QMap<QString, int> NewCharWizardPage2::calcCurrentRings(){
    QMap<QString, int> ringmap;
    QStringList ringlist = dal->qsl_getrings();
    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }

    //NOW - CALCULATE EXISTING RINGS
    //clan
    ringmap[dal->qs_getclanring(field("currentClan").toString())]++;
    //family
    ringmap[field("familyRing").toString()]++;
    //school
    //QStringList schoolrings = dal->qsl_getschoolrings(field("currentSchool").toString());
    QStringList schoolrings = field("ringChoices").toString().split("|");
    schoolrings.removeAll("");

    foreach (const QString r, schoolrings){
        ringmap[r]++;
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;
    qDebug() << ringmap;
    return ringmap;
}

QMap<QString, int> NewCharWizardPage2::calcSkills(){


    QStringList skills;
    skills.append(dal->qsl_getclanskills(field("currentClan").toString()));
    skills.append(dal->qsl_getfamilyskills(field("currentFamily").toString()));
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(field("q7skill").toString());
    skills.append(field("q8skill").toString());
    if(field("q13DisadvChecked").toBool() == true){
        qDebug() << "Question 13 chose disadvantage.  Adding skill: " << field("q13skill").toString();
        skills.append(field("q13skill").toString());
    }
    skills.append(field("parentSkill").toString());
    //get q18 skill


    int ancestorIndex = -1;
    QString heritage = "";
    if(field("ancestor1checked").toBool()){
        ancestorIndex = field("ancestor1index").toInt()+1;
        heritage = field("ancestor1").toString();
    }
    else if (field("ancestor2checked").toBool()){
        ancestorIndex = field("ancestor2index").toInt()+1;
        heritage = field("ancestor2").toString();
    }

    if(    //core
           heritage == Dictionary::Heritages::Wondrous_Work ||
           heritage == Dictionary::Heritages::Dynasty_Builder ||
           heritage == Dictionary::Heritages::Discovery ||
           heritage == Dictionary::Heritages::Ruthless_Victor ||
           heritage == Dictionary::Heritages::Elevated_for_Service ||
           //shadowlands
           heritage == Dictionary::Heritages::Infamous_Builder ||
           heritage == Dictionary::Heritages::Lost_in_the_Darkness ||
           heritage == Dictionary::Heritages::Vengeance_for_the_Fallen ||
           heritage == Dictionary::Heritages::Tewnty_Goblin_Thief
            ){
        skills.append(field("q18OtherEffects").toString());

    }

    skills.removeAll("");
    //initialize the skill map
    QMap<QString, int> skillmap;
    //skills start at 0.

    //clan
    foreach(const QString skill, skills){
        skillmap[skill]++;
    }

    return skillmap;
}


void NewCharWizardPage2::on_nc2_schoolSpecialtRing_ComboBox_currentIndexChanged(const QString &arg1)
{
    //if(ui->nc2_q4_lineEdit->text().isEmpty()){
        ui->nc2_q4_lineEdit->setText(dal->qs_getringdesc(arg1));
    //}
    regenSummary();

}
