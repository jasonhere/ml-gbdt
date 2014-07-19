#include "x.h"
#include "gbdt.h"

int main()
{
    XYSet set;
#if defined _MSC_VER
    load_liblinear("../data/heart_scale.txt", &set);
#else
    load_liblinear("./data/heart_scale.txt", &set);
#endif

    TreeParam param;
    param.verbose = 1;
    param.max_level = 5;
    param.max_leaf_number = 20;
    param.max_x_values_number = 200;
    param.leaf_threshold = 0.75;
    param.gbdt_tree_number = 400;
    param.gbdt_learning_rate = 0.1;
    param.gbdt_sample_rate = 0.9;

    GBDTTrainer trainer(set, param);
    trainer.train();

    FILE * output = yfopen("output.json", "w");
    trainer.save_json(output);
    fclose(output);

    GBDTPredictor predictor;
    FILE * input = yfopen("output.json", "r");
    predictor.load_json(input);
    fclose(input);

    for (size_t i=0, s=set.size(); i<s; i++)
    {
        const XY& xy = set.get(i);
        const CompoundValueVector& X = xy.X();
        double y = xy.y();
        printf("%lf = %lf should be near to %lf\n",
            trainer.predict(X), predictor.predict(X), y);
    }

    return 0;
}