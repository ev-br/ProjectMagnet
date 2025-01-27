#include "mcmc.h"
#include <iostream>
#include <fstream>
#include <cassert>

//Lattice::Lattice() {};
std::random_device generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);
//std::mt19937 generator(1234);
std::random_device generators1;
std::uniform_int_distribution<int> distribution1(0, 5);
//std::mt19937 generators1(123);
std::random_device generators2;
std::uniform_int_distribution<int> distribution2(0, 1);
//std::mt19937 generators3(12377);
std::random_device generators3;


Lattice::Lattice(long int max_seq_size) {
    lattice_side = max_seq_size;
    //��������� ���������� ������ ������� �� ���������� �������
    long int x, y;
    div_t n;
    map_of_contacts_int.resize(lattice_side * lattice_side * ndim2());
    for (long int i = 0; i < lattice_side * lattice_side; i++) {
        map_of_contacts_int[ndim2() * i] = i + 1;
        map_of_contacts_int[ndim2() * i + 1] = i - 1;
        map_of_contacts_int[ndim2() * i + 2] = i + lattice_side;
        map_of_contacts_int[ndim2() * i + 3] = i - lattice_side;
        map_of_contacts_int[ndim2() * i + 4] = i - lattice_side + 1;
        map_of_contacts_int[ndim2() * i + 5] = i + lattice_side - 1;
        n = div(i, lattice_side);
        x = n.rem;
        y = n.quot;
        for (int j = 0; j < ndim2(); j++) {
            if (x == 0) {
                map_of_contacts_int[ndim2() * i + 1] = i + lattice_side - 1;
            }
            if (x == (lattice_side - 1)) {
                map_of_contacts_int[ndim2() * i] = i - (lattice_side - 1);
            }
            if (y == 0) {
                map_of_contacts_int[ndim2() * i + 3] = lattice_side * (lattice_side - 1) + x;
            }
            if (y == (lattice_side - 1)) {
                map_of_contacts_int[ndim2() * i + 2] = x;
            }
            if ((y == 0) && (x == (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = lattice_side * (lattice_side - 1);
            }
            if ((y == 0) && (x < (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = lattice_side * (lattice_side - 1) + x + 1;
            }
            if ((y > 0) && (x == (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = (y - 1) * lattice_side;
            }
            if ((y == lattice_side - 1) && (x == 0)) {
                map_of_contacts_int[ndim() * i + 5] = lattice_side - 1;
            }
            if ((y == lattice_side - 1) && (x > 0)) {
                map_of_contacts_int[ndim() * i + 5] = x - 1;
            }
            if ((y < lattice_side - 1) && (x == 0)) {
                map_of_contacts_int[ndim() * i + 5] = i + 2 * lattice_side - 1;
            }
        }
    }
}

void Lattice::create_lattice(long int max_seq_size) {

    lattice_side = max_seq_size;
    long int x, y;
    div_t n;
    map_of_contacts_int.resize(lattice_side * lattice_side * ndim2());
    x_coords.resize(lattice_side * lattice_side * ndim2());
    y_coords.resize(lattice_side * lattice_side * ndim2());
    for (long int i = 0; i < lattice_side * lattice_side; i++) {
        map_of_contacts_int[ndim2() * i] = i + 1;
        map_of_contacts_int[ndim2() * i + 1] = i - 1;
        map_of_contacts_int[ndim2() * i + 2] = i + lattice_side;
        map_of_contacts_int[ndim2() * i + 3] = i - lattice_side;
        map_of_contacts_int[ndim2() * i + 4] = i - lattice_side + 1;
        map_of_contacts_int[ndim2() * i + 5] = i + lattice_side - 1;
        n = div(i, lattice_side);
        x = n.rem;
        y = n.quot;
        for (int j = 0; j < ndim2(); j++) {
            if (x == 0) {
                map_of_contacts_int[ndim2() * i + 1] = i + lattice_side - 1;
            }
            if (x == (lattice_side - 1)) {
                map_of_contacts_int[ndim2() * i] = i - (lattice_side - 1);
            }
            if (y == 0) {
                map_of_contacts_int[ndim2() * i + 3] = lattice_side * (lattice_side - 1) + x;
            }
            if (y == (lattice_side - 1)) {
                map_of_contacts_int[ndim2() * i + 2] = x;
            }
            if ((y == 0) && (x == (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = lattice_side * (lattice_side - 1);
            }
            if ((y == 0) && (x < (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = lattice_side * (lattice_side - 1) + x + 1;
            }
            if ((y > 0) && (x == (lattice_side - 1))) {
                map_of_contacts_int[ndim() * i + 4] = (y - 1) * lattice_side;
            }
            if ((y == lattice_side - 1) && (x == 0)) {
                map_of_contacts_int[ndim() * i + 5] = lattice_side - 1;
            }
            if ((y == lattice_side - 1) && (x > 0)) {
                map_of_contacts_int[ndim() * i + 5] = x - 1;
            }
            if ((y < lattice_side - 1) && (x == 0)) {
                map_of_contacts_int[ndim() * i + 5] = i + 2 * lattice_side - 1;
            }
        }
        x_coords[i] = x;
        y_coords[i] = y;



    }

}

Protein::Protein() {}

Protein::Protein(long int n) {

    /** type | previous | next   **/
    lattice.create_lattice(n + 5); //�������� �������, �� 5 ������, ��� ����� �������
    // 0 - ���������� �������� �� �������
    sequence_on_lattice.resize(lattice.lattice_side * lattice.lattice_side, 0); //������������������ ���������
    next_monomers.resize(lattice.lattice_side * lattice.lattice_side, -1); //�����-������ �� ��������� ����
    previous_monomers.resize(lattice.lattice_side * lattice.lattice_side, -1); //�����-������ �� ����������� ����

    directions.resize(lattice.lattice_side * lattice.lattice_side, -1); //����������� �� {0,1,2,3}

    ordered_coords.resize(lattice.lattice_side * lattice.lattice_side, -1); //���� ��� ��� ���������������� ���������

    number_of_monomers = n;
    start_conformation = 0;
    end_conformation = n - 1;

    sum_X = 0;
    sum_Y = 0;
    for (int i = 1; i < n - 1; i++)
    {
        previous_monomers[i] = i - 1;
        sequence_on_lattice[i] = 1;
        next_monomers[i] = i + 1;

        ordered_coords[i] = i;

        sum_X += i;
    }
    sum_X = sum_X + n - 1;
    ordered_coords[0] = 0;
    ordered_coords[n - 1] = n - 1;

    sequence_on_lattice[0] = 1;
    sequence_on_lattice[end_conformation] = 1; //��������� ������������������

    next_monomers[0] = 1;
    previous_monomers[n - 1] = n - 2;

    current_H_counts = n;
    E = -(n - 1);


    //������� ��� ����������� - �������� ������
    for (int i = 0; i < n - 1; i++)
    {
        directions[i] = 0;
    }

    bulk2_now = number_of_monomers - 2;

}

void Protein::count_contacts()
{
    long int hh = 0;
    long int current_position = start_conformation;
    coord_t  step;
    long int mag = 0;
    for (int i = 0; i < number_of_monomers; i++) {
        for (int j = 0; j < lattice.ndim2(); j++) {
            step = lattice.map_of_contacts_int[lattice.ndim2() * current_position + j];
            if (sequence_on_lattice[step] != 0)
            {
                hh = hh + sequence_on_lattice[current_position] * sequence_on_lattice[step];
            }
        }
        mag = mag + sequence_on_lattice[current_position];
        current_position = next_monomers[current_position];
    }

    E = -(hh / 2);
    current_H_counts = mag;
}


bool Protein::IsEndInStuck()
{
    int hh = 0;
    coord_t  step;
    for (int j = 0; j < lattice.ndim2(); j++) {
        step = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + j];
        if (sequence_on_lattice[step] != 0) {
            hh = hh + 1;
        }
    }
    return hh == lattice.ndim2();
}

void Protein::Reconnect1(int j) {
    int inverse_steps[4] = { 1, 0, 3, 2 };
    int reflect_directions[4][4] =
    { {3, 2, 0, 1}, //90
     {1, 0, 3, 2}, //180
     {2, 3, 1, 0}, //270
     {0, 1, 2, 3}
    };

    //int j = directions[previous_monomers[end_conformation]]; //����������� ���������� �����

    long int step = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + j];
    long int new_end = next_monomers[step];

    next_monomers[step] = end_conformation;

    directions[step] = inverse_steps[j];
    directions[new_end] = -1;

    long int c = end_conformation;
    long int new_c;
    while (c != new_end)
    {
        new_c = previous_monomers[c];

        next_monomers[c] = previous_monomers[c];
        //previous_monomers[new_c]=c;
        directions[c] = inverse_steps[directions[new_c]];
        c = new_c;
    }
    long int temp_prev_next = next_monomers[new_end];

    previous_monomers[end_conformation] = step;
    c = end_conformation;

    while (c != new_end)
    {
        new_c = next_monomers[c];
        previous_monomers[new_c] = c;
        c = new_c;
    }

    end_conformation = new_end;


    previous_monomers[new_end] = temp_prev_next;
    next_monomers[new_end] = -1;

}

void Protein::calc_bulk()
{
    bulk4_now = 0, bulk3_now = 0, bulk2_now = 0;

    long int current = start_conformation;
    long int step;
    int k = 0;
    for (int e = 0; e < number_of_monomers; e++)
    {
        k = 0;
        for (int j = 0; j < lattice.ndim2(); j++) {
            step = lattice.map_of_contacts_int[lattice.ndim2() * current + j];
            if (sequence_on_lattice[step] != 0) {
                k += 1;

            }
        }

        if (k == 2) {
            bulk2_now += 1;
        }
        if (k == 3) {
            bulk3_now += 1;
        }
        if (k == 4) {
            bulk4_now += 1;
        }
        if (k == 5) {
            bulk5_now += 1;
        }
        if (k == 6) {
            bulk6_now += 1;
        }

        current = next_monomers[current];

    }


    //std::cout << current << " ";

}





void Protein::Reconnect(int j) {
    int inverse_steps[6] = { 1, 0, 3, 2, 5, 4 };
    //    int reflect_directions[4][4] =
    //            {{3, 2, 0, 1}, //90
    //             {1, 0, 3, 2}, //180
    //             {2, 3, 1, 0}, //270
    //             {0, 1, 2, 3}
    //            };
    long int c;

    long int step = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + j];
    long int new_end = next_monomers[step];

    next_monomers[step] = end_conformation;

    directions[step] = inverse_steps[j];

    c = end_conformation;

    //std::cout << " end_conformation " <<  end_conformation << std::endl;
    if (end_conformation > lattice.lattice_side * lattice.lattice_side)
    {
        std::cout << " HZ why " << std::endl;
        return;

    }
    long int new_c;
    while (c != new_end)
    {
        // std::cout << "c  " << c << std::endl;
        assert(c >= 0 && c < lattice.lattice_side* lattice.lattice_side);
        if (c > lattice.lattice_side * lattice.lattice_side)
        {
            std::cout << " HZ why " << std::endl;
            return;
        }

        if (previous_monomers[c] > lattice.lattice_side * lattice.lattice_side)
        {
            std::cout << "nor  prev " << std::endl;
        }

        new_c = previous_monomers[c];

        if (new_c < 0 || c > lattice.lattice_side * lattice.lattice_side)
        {
            std::cout << " we have problems " << std::endl;
        }

        next_monomers[c] = previous_monomers[c];


        directions[c] = inverse_steps[directions[new_c]];
        c = new_c;
    }
    long int temp_prev_next = next_monomers[new_end];


    previous_monomers[end_conformation] = step;
    c = end_conformation;


    while (c != new_end)
    {
        new_c = next_monomers[c];
        previous_monomers[new_c] = c;
        c = new_c;
    }

    end_conformation = new_end;


    //std::cout <<"mmmm" << " " << step << std::endl;

    previous_monomers[new_end] = temp_prev_next;
    next_monomers[new_end] = -1;
    directions[new_end] = -1;
}


void Protein::MC(double J_in, double h_in, int Simulation, long int steps_to_equilibrium, long int mc_steps, bool bradius)
{
    nSimulation = Simulation;
    J = J_in;
    h = h_in;
    //[i,j]: i-�������, j-����������� (������ �������)
    int reflect_directions[4][4] =
    { {3, 2, 0, 1 }, //90
     {1,0,3,2 }, //180
     {2,3, 1, 0 }, //270
    { 0, 1, 2, 3}
    };

    int inverse_steps[6] = { 1,0,3,2,5,4 }; //��� ���������� ����������� � ������� "��������� ����� � ������"
    //double step_rd; //��� ������ �������: ������� ��� ���������
    double q_rd, p1, p_metropolis; //��� ����������� �������� ����
    int rand_path; // = distribution1(generators1); //���������� �����������: 0 - ������������ ������ � �����
    double typeOfUpdate; //0 - �������; 1 - ���������
    long int step;
    int step_on_lattice;//����� ������ �� �������
    long int new_point;
    long int new_E, new_H;
    int hh;
    long int temp, del, oldspin;


    //std::uniform_int_distribution<long int> distribution_spin(0, number_of_monomers-1);
    std::uniform_int_distribution<long int> distribution_spin(0, lattice.lattice_side * lattice.lattice_side - 1);
    //std::mt19937 generator_spin(123);
    std::random_device generator_spin;

    //����������� �������� ���� � ������� � ���������� �������
    P_add = 1 - exp(-2 * J); //���� ��� ��� h=0

    double p_for_local_update = 0.6;
    double p_for_reconnect = 1.0; //p_for_local_update - p_for_reconnect = ����������� ����������

    //spins_in_cluster.resize(number_of_monomers, false);

    long int all_steps = steps_to_equilibrium + mc_steps;

    for (long int i = 0; i < all_steps + 2; i++) {
        //std::cout << "STEP : " << i << std::endl;
        typeOfUpdate = distribution(generator);
        if (typeOfUpdate < p_for_local_update) {
            hh = 0;
            rand_path = distribution2(generators3);

            if (rand_path == 0) {//������������ ������ � �����

                step_on_lattice = distribution1(generators1);
                new_point = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + step_on_lattice];
                oldspin = sequence_on_lattice[start_conformation];

                if (sequence_on_lattice[new_point] == 0) { //��������, ��� � ���� ��� ���������

                    //������ ������

                    //��������� � �����
                    next_monomers[end_conformation] = new_point;
                    sequence_on_lattice[new_point] = 2 * distribution2(generators3) - 1; //����� �����
                    previous_monomers[new_point] = end_conformation;
                    end_conformation = new_point;

                    //������� ������
                    temp = start_conformation;
                    start_conformation = next_monomers[start_conformation];
                    next_monomers[temp] = -1;
                    previous_monomers[start_conformation] = -1;

                    //������� ������
                    for (int j = 0; j < lattice.ndim2(); j++) {
                        step = lattice.map_of_contacts_int[lattice.ndim2() * temp + j];
                        if (sequence_on_lattice[step] != 0) {
                            hh = hh - sequence_on_lattice[temp] * sequence_on_lattice[step];
                        }
                    }

                    //������� �������
                    for (int j = 0; j < lattice.ndim2(); j++) {
                        step = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + j];
                        if (sequence_on_lattice[step] != 0) {
                            hh = hh + sequence_on_lattice[end_conformation] * sequence_on_lattice[step];
                        }
                    }

                    new_E = E + hh;
                    //new_H = current_H_counts + sequence_on_lattice[new_point] - sequence_on_lattice[start_conformation];
                    new_H = current_H_counts + sequence_on_lattice[new_point] - sequence_on_lattice[temp];

                    p1 = exp(-(-(new_E - E) * J - (new_H - current_H_counts) * h));
                    p_metropolis = std::min(1.0, p1);
                    q_rd = distribution(generator);
                    if (q_rd < p_metropolis) {
                        E = new_E;
                        current_H_counts = new_H;
                        sequence_on_lattice[temp] = 0; //����� �����, ��� ��� ����� ������� �������(!!!)
                        sum_X = sum_X + lattice.x_coords[end_conformation] - lattice.x_coords[temp];
                        sum_Y = sum_Y + lattice.y_coords[end_conformation] - lattice.y_coords[temp];

                        //������������ ���������� � ������������
                        directions[temp] = -1;
                        directions[previous_monomers[end_conformation]] = step_on_lattice;

                        //�������������
                        //ordered_coords[temp] = -1;
                        //ordered_coords[new_point] = end_conformation;

                    }
                    else {//�������� ���������
                        //������� �����
                        del = end_conformation;
                        end_conformation = previous_monomers[end_conformation];
                        next_monomers[end_conformation] = -1;
                        previous_monomers[del] = -1;
                        sequence_on_lattice[del] = 0;

                        //���������� ������
                        previous_monomers[start_conformation] = temp;
                        next_monomers[temp] = start_conformation;
                        start_conformation = temp;
                        sequence_on_lattice[start_conformation] = oldspin;

                    }

                }
                else {//����� ���, ������� �� ����
                    //continue;
                }

            }
            else {//������������ ����� � ������

                step_on_lattice = distribution1(generators1);
                new_point = lattice.map_of_contacts_int[lattice.ndim2() * start_conformation + step_on_lattice];
                oldspin = sequence_on_lattice[end_conformation];

                if (sequence_on_lattice[new_point] == 0) { //��������, ��� � ���� ��� ���������

                    //������ ������
                    //��������� � ������
                    previous_monomers[start_conformation] = new_point;
                    sequence_on_lattice[new_point] = 2 * distribution2(generators3) - 1; //����� �����
                    next_monomers[new_point] = start_conformation;
                    start_conformation = new_point;

                    //������� �����
                    temp = end_conformation;
                    end_conformation = previous_monomers[end_conformation];
                    if (previous_monomers[end_conformation] < 0) {
                        std::cout << "problem update " << std::endl;
                    }
                    previous_monomers[temp] = -1;
                    next_monomers[end_conformation] = -1;

                    //������� ������
                    for (int j = 0; j < lattice.ndim2(); j++) {
                        step = lattice.map_of_contacts_int[lattice.ndim2() * temp + j];
                        if (sequence_on_lattice[step] != 0) {
                            hh = hh - sequence_on_lattice[temp] * sequence_on_lattice[step];
                        }
                    }

                    //������� �������
                    for (int j = 0; j < lattice.ndim2(); j++) {
                        step = lattice.map_of_contacts_int[lattice.ndim2() * start_conformation + j];
                        if (sequence_on_lattice[step] != 0) {
                            hh = hh + sequence_on_lattice[start_conformation] * sequence_on_lattice[step];
                        }
                    }

                    new_E = E + hh;
                    new_H = current_H_counts + sequence_on_lattice[new_point] - sequence_on_lattice[temp];

                    //p1 = exp(-(new_E - E) * J - (new_H - current_H_counts) * h);

                    p1 = exp(-(-(new_E - E) * J - (new_H - current_H_counts) * h));
                    p_metropolis = std::min(1.0, p1);
                    q_rd = distribution(generator);

                    if (q_rd < p_metropolis) {
                        E = new_E;
                        current_H_counts = new_H;
                        sequence_on_lattice[temp] = 0; //����� �����, ��� ��� ����� ������� �������(!!!)
                        sum_X = sum_X + lattice.x_coords[start_conformation] - lattice.x_coords[temp];
                        sum_Y = sum_Y + lattice.y_coords[start_conformation] - lattice.y_coords[temp];
                        //sum_X = sum_X + lattice.x_coords[start_conformation];
                        //sum_Y = sum_Y + lattice.y_coords[start_conformation];

                        //������������ ���������� � ������������
                        directions[end_conformation] = -1;
                        directions[start_conformation] = inverse_steps[step_on_lattice];

                    }
                    else {//�������� ���������
                     //������� ������
                        del = start_conformation;
                        start_conformation = next_monomers[start_conformation];
                        previous_monomers[start_conformation] = -1;
                        next_monomers[del] = -1;
                        sequence_on_lattice[del] = 0;

                        //���������� �����
                        next_monomers[end_conformation] = temp;
                        previous_monomers[temp] = end_conformation;
                        end_conformation = temp;
                        sequence_on_lattice[end_conformation] = oldspin;

                        if (previous_monomers[temp] < 0) {
                            std::cout << "problem return " << std::endl;
                        }
                        if (temp < 0) {
                            std::cout << "problem return temp" << std::endl;
                        }
                    }
                }
                else {
                    //������ ����
                }

            }
        }
        else {

            long int coord = distribution_spin(generator_spin);

            if (sequence_on_lattice[coord] != 0)
            { //����������� ������ ������� 1/n, ������ ���������� ������
                int sign = sequence_on_lattice[coord];

                std::valarray<bool> used_coords;
                used_coords.resize(lattice.lattice_side * lattice.lattice_side, false);

                std::queue<long int> Cluster;

                Cluster.push(coord);
                used_coords[coord] = true;

                while (!Cluster.empty()) {
                    temp = Cluster.front();
                    Cluster.pop();

                    for (int j = 0; j < lattice.ndim2(); j++)
                    {
                        step = lattice.map_of_contacts_int[lattice.ndim2() * temp + j];
                        double p = distribution(generator);
                        //???
                        if (sequence_on_lattice[step] == sign && p < P_add &&
                            !used_coords[step]) {
                            Cluster.push(step);
                            used_coords[step] = true;
                            sequence_on_lattice[step] *= -1;
                        }
                    }
                }
                sequence_on_lattice[coord] *= -1;

                count_contacts();

                //std::cout << "cluster type done " << std::endl;
            }
            else
            { //������ ����� �����, ����� ������� ������� ���������
                step_on_lattice = distribution1(generators1);
                new_point = lattice.map_of_contacts_int[lattice.ndim2() * end_conformation + step_on_lattice];

                //��������, ��� ����������� ���� ����� ������
                if (sequence_on_lattice[new_point] != 0 && next_monomers[new_point] != -1 &&
                    new_point != previous_monomers[end_conformation]) {
                    Reconnect(step_on_lattice);
                }
            }


        }


        if (i > steps_to_equilibrium && i % 10000000 == 0)
        {
            save_calcs();
            //radius();


            calc_bulk();
            bulk2 << 1.0 * bulk2_now / number_of_monomers;
            bulk3 << 1.0 * bulk3_now / number_of_monomers;
            bulk4 << 1.0 * bulk4_now / number_of_monomers;
            bulk5 << 1.0 * bulk3_now / number_of_monomers;
            bulk6 << 1.0 * bulk4_now / number_of_monomers;

            coord_form();

        }



        /*        if (i > steps_to_equilibrium) {
                    save_calcs();
                    radius();
                }
                if (i% number_of_monomers == 0)
                {
                    calc_bulk();
                    bulk2 << 1.0*bulk2_now/number_of_monomers;
                    bulk3 << 1.0*bulk3_now/number_of_monomers;
                    bulk4 << 1.0*bulk4_now/number_of_monomers;
                }
                //if (i%(number_of_monomers*number_of_monomers)==0)
                if (i > steps_to_equilibrium && i% number_of_monomers==0)
                {
                    coord_form();
                }*/


                //if ( i> steps_to_equilibrium && i%100000000==0 )
        if (i > steps_to_equilibrium && i % 100000000 == 0)
        {

            write_file(i);


        }


    }

}

/*
bool Protein::CheckAndFlipNodes (long int& coord, int& sign )
{
    for (int j = 0; j < lattice.ndim2(); j++) {
     long int step = lattice.map_of_contacts_int[lattice.ndim2() * coord + j];
       // std::cout << coord << "  start "<< step <<  std::endl;
        if (sequence_on_lattice[step] != 0) {
            if (sequence_on_lattice[step] ==sign )
            {
                double p = distribution(generator);
                if (p < P_add)
                {
                    CheckAndFlipNodes (step, sign );
                    spins_in_cluster.push(step);
                    //����� ���� � �������?
                    //spins_in_cluster[]
                }
            }
        }
    }
   // std::cout << coord << " finish function  " <<  std::endl;
return false;
}*/

/*void Protein::radius()
{
    long int point1x = end_conformation % lattice.lattice_side;
    long int point1y = end_conformation / lattice.lattice_side;
    long int point1xs = start_conformation % lattice.lattice_side;
    long int point1ys = start_conformation / lattice.lattice_side;
  //���������� �� ����
    long int xdiff = abs(point1x- point1xs);
    if (xdiff > (lattice.lattice_side  / 2))
    xdiff = lattice.lattice_side - xdiff;
    long int ydiff = abs(point1y- point1ys);
    if (ydiff > (lattice.lattice_side / 2))
    ydiff = lattice.lattice_side - ydiff;
    long int r = xdiff *xdiff  + ydiff*ydiff;
    dists << r;
}*/

long int Protein::radius()
{
    long int point1x = end_conformation % lattice.lattice_side;
    long int point1y = end_conformation / lattice.lattice_side;
    long int point1xs = start_conformation % lattice.lattice_side;
    long int point1ys = start_conformation / lattice.lattice_side;

    //���������� �� ����
    long int xdiff = abs(point1x - point1xs);
    if (xdiff > (lattice.lattice_side / 2))
        xdiff = lattice.lattice_side - xdiff;

    long int ydiff = abs(point1y - point1ys);
    if (ydiff > (lattice.lattice_side / 2))
        ydiff = lattice.lattice_side - ydiff;

    long int r = xdiff * xdiff + ydiff * ydiff;
    dists << r;
    return r;
}



void Protein::radius_gyration()
{


    long double r_g = 0;
    long int current = start_conformation;
    long double y = 0, x = 0;
    long double point1x = 0, point1y = 0;
    //long double point1x = 1.0*sum_X/number_of_monomers;
    //long double point1y = 1.0*sum_Y/number_of_monomers;
    long double xdiff, ydiff;
    //point1x = start_conformation % lattice.lattice_side;
    //point1y = start_conformation / lattice.lattice_side;

    current = start_conformation;
    long double r;
    long int point1xs, point1ys;

    long second_current;
    for (int e = 0; e < number_of_monomers; e++)
    {
        second_current = start_conformation;

        point1x = lattice.x_coords[current];
        point1y = lattice.y_coords[current];

        for (int e1 = 0; e1 < number_of_monomers; e1++) {

            point1xs = lattice.x_coords[second_current];
            point1ys = lattice.y_coords[second_current];

            //���������� �� ����
            xdiff = abs(point1x - point1xs);
            if (xdiff > (lattice.lattice_side / 2))
                xdiff = lattice.lattice_side - xdiff;

            ydiff = abs(point1y - point1ys);
            if (ydiff > (lattice.lattice_side / 2))
                ydiff = lattice.lattice_side - ydiff;

            r = xdiff * xdiff + ydiff * ydiff;

            r_g = r_g + r;

            second_current = next_monomers[second_current];

        }
        current = next_monomers[current];

        //std::cout << current << " ";

    }
    //std::cout << std::endl;
    gyration << 0.5 * r_g / number_of_monomers / number_of_monomers;

}



void Protein::coord_form() {

    std::vector <long long int> xs, ys;
    xs.push_back(0);
    ys.push_back(0);

    int direction;

    long int current = start_conformation;


    std::vector <std::vector<int>> steps = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    for (long long int i = 1; i < number_of_monomers; i++)
    {
        direction = directions[current];

        long long int x = xs.back() + steps[direction][0];
        long long int y = ys.back() + steps[direction][1];

        xs.push_back(x);
        ys.push_back(y);

        current = next_monomers[current];


    }

    long double r_g = 0;
    long double r, r_notdig;
    long double y = 0, x = 0;
    long double point1x = 0, point1y = 0;
    //long double point1x = 1.0*sum_X/number_of_monomers;
    //long double point1y = 1.0*sum_Y/number_of_monomers;
    long double xdiff, ydiff;
    double  A_element = 0, D_element = 0, BC_element2 = 0;


    long long int second_current;
    for (int e = 0; e < number_of_monomers; e++)
    {
        second_current = start_conformation;



        for (int e1 = 0; e1 < number_of_monomers; e1++) {


            //���������� �� ����

            xdiff = xs[e1] - xs[e];
            ydiff = ys[e1] - ys[e];

            r = xdiff * xdiff + ydiff * ydiff;
            r_g = r_g + r;



            A_element += xdiff * xdiff;
            D_element += ydiff * ydiff;
            BC_element2 += xdiff * ydiff;
            //r_notdig = xdiff*ydiff;
            //NoDiagnalEleents = NoDiagnalEleents + r_notdig;



            second_current = next_monomers[second_current];

        }
        current = next_monomers[current];


        //std:: cout << eigs1.mean() << " " << eigs2.mean() << " " << aratio.mean()  << " " << gyration.mean() << std::endl;
        //std::cout << current << " ";

    }

    gyration << 0.5 * r_g / number_of_monomers / number_of_monomers;
    A_element = 1.0 * A_element / number_of_monomers / number_of_monomers / 2.0;
    D_element = 1.0 * D_element / number_of_monomers / number_of_monomers / 2.0;

    BC_element2 = 1.0 * BC_element2 / number_of_monomers / number_of_monomers / 2.0;

    double D = (A_element + D_element) * (A_element + D_element) - 4 * (A_element * D_element - BC_element2 * BC_element2);

    double eigvals1 = ((A_element + D_element) + sqrt(D)) * 0.5;

    double eigvals2 = ((A_element + D_element) - sqrt(D)) * 0.5;

    eigs1 << eigvals1;
    eigs2 << eigvals2;

    aratio << 1.0 * (eigvals1 - eigvals2) * (eigvals1 - eigvals2) / ((eigvals1 + eigvals2) * (eigvals1 + eigvals2));



    //std::cout << " Compare from coords " << std:: endl;
    //std::cout << A_element+D_element <<  " - " << 0.5*r_g/number_of_monomers/number_of_monomers << std::endl ;
    //std:: cout << eigs1.mean() << " " << eigs2.mean() << " " << aratio.mean()  << " " << gyration.mean() << std::endl;
}

void Protein::write_file(long int i) {

    std::string filename;
    std::ofstream out_result;

    filename = "Geometry_Ising_" + std::to_string(J) + "_" + std::to_string(h) + "_" + std::to_string(number_of_monomers) + ".txt";
    //filename = "Radius_"+std::to_string(J)+"_"+std::to_string(number_of_monomers)+"_CanonicalIsing.txt";

    out_result.open(filename);
    //out_result << mc_steps<<" " << number_of_monomers << " " << J << " " << h  <<   " ";
    out_result << "N J h mean_R_sq err_mean_R_sq mean_R_gyr_sq err_mean_R_gyr_sq ";
    out_result << "lambda1 err_lambda1 lambda2 err_lambda2 acperical err_aspherical";
    out_result << "bulk2 err_bulk2 bulk3 err_bulk3 bulk4 err_bulk4 bulk5 err_bulk5 bulk6 err_bulk6" << " steps" << std::endl;

    out_result << number_of_monomers << " " << J << " " << h << " ";
    out_result << dists.mean() << " " << dists.errorbar() << " " << gyration.mean() << " " << gyration.errorbar() << " ";

    out_result << eigs1.mean() << " " << eigs1.errorbar() << " ";
    out_result << eigs2.mean() << " " << eigs2.errorbar() << " ";
    out_result << aratio.mean() << " " << aratio.errorbar() << " ";

    out_result << bulk2.mean() << " " << bulk2.errorbar() << " ";
    out_result << bulk3.mean() << " " << bulk3.errorbar() << " ";
    out_result << bulk4.mean() << " " << bulk4.errorbar() << " ";
    out_result << bulk5.mean() << " " << bulk5.errorbar() << " ";
    out_result << bulk6.mean() << " " << bulk6.errorbar() << " ";

    out_result << i << " ";

    out_result << std::endl;

    out_result.close();


    out_result.close();



    filename = "BC_" + std::to_string(J) + "_" + std::to_string(h) + "_" + std::to_string(number_of_monomers) + "_" + std::to_string(nSimulation) + ".txt";
    //filename = "Radius_"+std::to_string(J)+"_"+std::to_string(number_of_monomers)+"_CanonicalIsing.txt";

    out_result.open(filename);
    //out_result << mc_steps<<" " << number_of_monomers << " " << J << " " << h  <<   " ";
    out_result << "N J h mean_R_sq err_mean_R_sq mean_R_gyr_sq err_mean_R_gyr_sq ";
    out_result << "mean_e err_mean_e mean_e_sq err_mean_e_sq mean_e_fourth err_mean_e_fourth ";
    out_result << "mean_m err_mean_m mean_m_sq err_mean_m_sq mean_m_fourth err_mean_m_fourth " << std::endl;

    out_result << number_of_monomers << " " << J << " " << h << " ";
    out_result << dists.mean() << " " << dists.errorbar() << " " << gyration.mean() << " " << gyration.errorbar() << " ";

    out_result << energy.mean() << " " << energy.errorbar() << " ";
    out_result << energy_sq.mean() << " " << energy_sq.errorbar() << " ";
    out_result << energy_4.mean() << " " << energy_4.errorbar() << " ";

    out_result << magnetization.mean() << " " << magnetization.errorbar() << " ";
    out_result << magnetization_sq.mean() << " " << magnetization_sq.errorbar() << " ";
    out_result << magnetization_4.mean() << " " << magnetization_4.errorbar() << " ";
    out_result << i << " ";

    out_result << std::endl;

    out_result.close();


    out_result.close();

}



void Protein::radius_gyration1()
{
    long double r_g = 0;
    long int current = start_conformation;
    long double y = 0, x = 0;
    long double point1x = 0, point1y = 0;
    //long double point1x = 1.0*sum_X/number_of_monomers;
    //long double point1y = 1.0*sum_Y/number_of_monomers;
    long double xdiff, ydiff;
    point1x = start_conformation % lattice.lattice_side;
    point1y = start_conformation / lattice.lattice_side;

    for (int e = 0; e < number_of_monomers; e++)
    {

        long int point1xs = lattice.x_coords[current];
        long int point1ys = lattice.y_coords[current];

        //���������� �� ����
        xdiff = abs(point1x - point1xs);
        if (xdiff > (lattice.lattice_side / 2))
            xdiff = lattice.lattice_side - xdiff;

        ydiff = abs(point1y - point1ys);
        if (ydiff > (lattice.lattice_side / 2))
            ydiff = lattice.lattice_side - ydiff;

        x = x + xdiff;
        y = y + ydiff;

        //r = xdiff *xdiff  + ydiff*ydiff;

        current = next_monomers[current];

        //std::cout << current << " ";

    }

    point1x = x / number_of_monomers;
    point1y = y / number_of_monomers;

    current = start_conformation;
    long double r;
    long int point1xs, point1ys;
    for (int e = 0; e < number_of_monomers; e++)
    {

        point1xs = lattice.x_coords[current];
        point1ys = lattice.y_coords[current];

        //���������� �� ����
        xdiff = abs(point1x - point1xs);
        if (xdiff > (lattice.lattice_side / 2))
            xdiff = lattice.lattice_side - xdiff;

        ydiff = abs(point1y - point1ys);
        if (ydiff > (lattice.lattice_side / 2))
            ydiff = lattice.lattice_side - ydiff;

        r = xdiff * xdiff + ydiff * ydiff;

        r_g = r_g + r;


        current = next_monomers[current];

        //std::cout << current << " ";

    }
    //std::cout << std::endl;
    gyration << 1.0 * r_g / number_of_monomers;

}

void Protein::save_calcs()
{

    energy << 1.0 * (E) / number_of_monomers;
    energy_sq << 1.0 * (E) / number_of_monomers * 1.0 * (E) / number_of_monomers;
    energy_4 << 1.0 * (E) / number_of_monomers * 1.0 * (E) / number_of_monomers * 1.0 * (E) / number_of_monomers * 1.0 * (E) / number_of_monomers;

    magnetization << 1.0 * abs(current_H_counts) / number_of_monomers;
    magnetization_sq << 1.0 * current_H_counts / number_of_monomers * 1.0 * current_H_counts / number_of_monomers;
    magnetization_4 << 1.0 * current_H_counts / number_of_monomers * 1.0 * current_H_counts / number_of_monomers * 1.0 * current_H_counts / number_of_monomers * 1.0 * current_H_counts / number_of_monomers;


    count_E[E] = count_E[E] + 1;
    count_M[current_H_counts] = count_M[current_H_counts] + 1;

    radius();


}